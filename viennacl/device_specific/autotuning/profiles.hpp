#ifndef VIENNACL_DEVICE_SPECIFIC_PROFILES_HPP
#define VIENNACL_DEVICE_SPECIFIC_PROFILES_HPP

/* =========================================================================
   Copyright (c) 2010-2013, Institute for Microelectronics,
           Institute for Analysis and Scientific Computing,
           TU Wien.
   Portions of this software are copyright by UChicago Argonne, LLC.

           -----------------
 ViennaCL - The Vienna Computing Library
           -----------------

   Project Head:    Karl Rupp  rupp@iue.tuwien.ac.at

   (A list of authors and contributors can be found in the PDF manual)

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */


/** @file viennacl/generator/profiles.hpp
 *
 * Vendor-specific parameters for the generated kernels
*/

#include <map>

#include "viennacl/ocl/device.hpp"

#include "viennacl/device_specific/forwards.h"

#include "viennacl/tools/shared_ptr.hpp"

#include "viennacl/device_specific/templates/template_base.hpp"
#include "viennacl/device_specific/templates/saxpy.hpp"
#include "viennacl/device_specific/templates/scalar_reduction.hpp"
#include "viennacl/device_specific/templates/vector_reduction.hpp"
#include "viennacl/device_specific/templates/matrix_product.hpp"

namespace viennacl{

  namespace device_specific{

    namespace profiles{

      using namespace viennacl::ocl;

      typedef cl_uint vendor_id_type;
      typedef cl_device_type device_type;

      typedef std::string device_name_type;
      typedef tools::shared_ptr<profile_base> profile_base_ptr;

      template<class KeyType, class ValueType>
      struct map_wrapper{
          typedef std::map<KeyType,ValueType> map_type;
          map_type map;
          ValueType & operator[](KeyType const & key){ return map[key]; }
      };

      struct expression_map : public map_wrapper<expression_key_type, profile_base_ptr>{ };
      struct device_name_map : public map_wrapper<device_name_type, expression_map>{ };
      struct device_architecture_map : public map_wrapper<device_architecture_family, device_name_map>{ };
      struct device_type_map : public map_wrapper<device_type,device_architecture_map>{ };
      struct database_type : public map_wrapper<vendor_id_type, device_type_map>{ };

      /** @brief Set a default of a generation to a particular device for a particular operation */
        inline void set_generation_default_to(database_type & map, vendor_id_type vendor_id, device_architecture_family family, expression_key_type expression, std::string const & device_name){
            map[vendor_id][CL_DEVICE_TYPE_GPU][family][""][expression] = map[vendor_id][CL_DEVICE_TYPE_GPU][family][device_name][expression];
        }

        /** @brief Set a default of a generation to a particular device for all operations */
        inline void set_all_generation_default_to(database_type & map, vendor_id_type vendor_id, device_architecture_family family, std::string const & device_name){
            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_SAXPY_TYPE,FLOAT_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_SAXPY_TYPE,FLOAT_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(SCALAR_REDUCE_TYPE,FLOAT_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_REDUCE_Tx_TYPE,FLOAT_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_REDUCE_Nx_TYPE,FLOAT_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_TT_TYPE,FLOAT_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_NT_TYPE,FLOAT_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_TN_TYPE,FLOAT_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_NN_TYPE,FLOAT_TYPE),device_name);

            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_SAXPY_TYPE,DOUBLE_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_SAXPY_TYPE,DOUBLE_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(SCALAR_REDUCE_TYPE,DOUBLE_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_REDUCE_Tx_TYPE,DOUBLE_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(VECTOR_REDUCE_Nx_TYPE,DOUBLE_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_TT_TYPE,DOUBLE_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_NT_TYPE,DOUBLE_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_TN_TYPE,DOUBLE_TYPE),device_name);
            set_generation_default_to(map,vendor_id,family,std::make_pair(MATRIX_PRODUCT_NN_TYPE,DOUBLE_TYPE),device_name);
        }

        /** @brief Initialize the database */
      static database_type init_database(){
        database_type map;

        /*---------------------------*/
        /*     GPU Defaults          */
        /*---------------------------*/
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_saxpy("float",1,128,128,true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_saxpy("float",1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,FLOAT_TYPE)] = profile_base_ptr(new scalar_reduction("float",1, 128, 128, true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'T',1, 1, 256, 32));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'N',1, 1, 256, 32));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','T',1,8,32,8,4,4,4,1,0,8,8));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','T',1,8,32,8,4,4,4,1,0,8,8));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','N',1,8,32,8,4,4,4,1,0,8,8));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','N',1,8,32,8,4,4,4,1,0,8,8));


        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_saxpy("double",1,128,128,true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_saxpy("double",1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new scalar_reduction("double",1, 128, 128, true));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'T',1, 1, 256, 32));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'N',1, 1, 256, 32));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','T',1,8,32,8,4,4,4,1,0,8,8));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','T',1,8,32,8,4,4,4,1,0,8,8));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','N',1,8,32,8,4,4,4,1,0,8,8));
        map[unknown_id][CL_DEVICE_TYPE_GPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','N',1,8,32,8,4,4,4,1,0,8,8));

        /*---------------------------*/
        /*     CPU Defaults          */
        /*---------------------------*/
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_saxpy("float",8,16,256,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_saxpy("float",1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,FLOAT_TYPE)] = profile_base_ptr(new scalar_reduction("float",8,8,512,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'T',1,2,1,8));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'N',1,16,8,8));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','T',4,16,64,1,4,4,128,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','T',4,16,64,1,4,4,128,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','N',4,16,64,1,4,4,128,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','N',4,16,64,1,4,4,128,0,0,0,0));


        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_saxpy("double",8,16,32,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_saxpy("double",1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new scalar_reduction("double",8,8,512,true));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'T',1,1,1,8));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'N',1,8,16,16));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','T',2,16,64,1,8,4,64,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','T',2,16,64,1,8,4,64,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','N',2,16,64,1,8,4,64,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_CPU][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','N',2,16,64,1,8,4,64,0,0,0,0));


        /*---------------------------*/
        /*     ACCELERATOR Defaults  */
        /*---------------------------*/
        //same as CPU for now
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_saxpy("float",8,16,256,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_saxpy("float",1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,FLOAT_TYPE)] = profile_base_ptr(new scalar_reduction("float",8,8,512,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'T',1,2,1,8));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'N',1,16,8,8));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','T',4,16,64,1,4,4,128,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','T',1,16,64,1,16,4,32,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','N',1,16,64,1,16,4,32,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','N',1,16,64,1,16,4,32,0,0,0,0));


        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_saxpy("double",8,16,32,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_saxpy("double",1,16,16,16,16,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(SCALAR_REDUCE_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new scalar_reduction("double",8,8,512,true));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Tx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'T',1,1,1,8));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(VECTOR_REDUCE_Nx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'N',1,8,16,16));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','T',2,16,64,1,8,4,64,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','T',1,16,128,2,8,4,16,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_TN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','N',1,16,64,1,16,4,32,0,0,0,0));
        map[unknown_id][CL_DEVICE_TYPE_ACCELERATOR][UNKNOWN][""][std::make_pair(MATRIX_PRODUCT_NN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','N',1,16,64,1,16,4,32,0,0,0,0));



        /*---------------------------*/
        /*     AMD                   */
        /*---------------------------*/

        //Evergreen

            //Cypress
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_saxpy("float",1,4,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_saxpy("float",1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(SCALAR_REDUCE_TYPE,FLOAT_TYPE)] = profile_base_ptr(new scalar_reduction("float",8,128,128,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'T',1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'N',1,32,8,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','T',4,4,64,16,4,4,8,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','T',2,8,32,8,4,1,4,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','N',4,8,64,16,4,4,8,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','N',4,8,128,8,8,4,4,0,0,0,0));

            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_saxpy("double",2,1,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_saxpy("double",1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(SCALAR_REDUCE_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new scalar_reduction("double",2,256,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'T',1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'N',1,64,4,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','T',2,16,32,16,2,2,8,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','T',2,8,32,8,4,1,4,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','N',4,2,64,32,8,8,4,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][Evergreen]["Cypress"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','N',2,16,64,8,2,2,4,0,0,0,0));

            //Default
            set_all_generation_default_to(map,amd_id,Evergreen,"Cypress");


         //Southern Islands
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_saxpy("float",1,4,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_saxpy("float",1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(SCALAR_REDUCE_TYPE,FLOAT_TYPE)] = profile_base_ptr(new scalar_reduction("float",8,128,128,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'T',1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'N',1,32,8,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','T',4,8,128,32,4,4,4,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','T',2,8,32,8,4,2,4,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','N',4,16,64,16,4,4,8,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','N',4,16,64,16,4,4,8,0,0,0,0));

            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_saxpy("double",2,1,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_saxpy("double",1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(SCALAR_REDUCE_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new scalar_reduction("double",2,256,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'T',1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'N',1,64,4,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','T',2,4,128,64,4,2,2,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','T',2,8,32,8,4,2,4,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','N',2,8,128,32,2,2,2,0,0,0,0));
            map[amd_id][CL_DEVICE_TYPE_GPU][SouthernIslands]["Tahiti"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','N',2,8,128,32,2,2,2,0,0,0,0));


            //Default
            set_all_generation_default_to(map,amd_id,SouthernIslands,"Tahiti");
			
			
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_saxpy("float",1,4,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_SAXPY_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_saxpy("float",1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(SCALAR_REDUCE_TYPE,FLOAT_TYPE)] = profile_base_ptr(new scalar_reduction("float",8,128,128,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'T',1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,FLOAT_TYPE)] = profile_base_ptr(new vector_reduction("float",'N',1,32,8,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','T',1,16,16,8,4,1,8,1,1,16,8));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','T',1,16,16,8,4,1,8,1,1,32,4));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','N',1,16,16,16,8,1,4,1,1,16,16));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','N',1,16,16,8,4,1,8,1,1,16,8));

            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_saxpy("double",2,1,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_SAXPY_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_saxpy("double",1,16,16,16,16,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(SCALAR_REDUCE_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new scalar_reduction("double",2,256,64,true));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'T',1,1,256,1024));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new vector_reduction("double",'N',1,64,4,256));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','T',1,16,16,16,4,1,8,1,1,16,16));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','T',1,16,16,16,4,1,4,1,1,32,8));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','N',1,16,16,16,4,1,4,1,1,16,16));
            map[amd_id][CL_DEVICE_TYPE_GPU][VolcanicIslands]["Hawaii"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','N',1,16,16,16,4,1,4,1,1,16,16));
            set_all_generation_default_to(map,amd_id,VolcanicIslands,"Hawaii");


        /*---------------------------*/
        /*     NVidia                */
        /*---------------------------*/

        //-----Fermi

          //Geforce GTX 470
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_SAXPY_TYPE,FLOAT_TYPE)]      =    profile_base_ptr(new vector_saxpy("float",1,1,256,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_SAXPY_TYPE,FLOAT_TYPE)]      =    profile_base_ptr(new matrix_saxpy("float",1,16,16,16,16,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(SCALAR_REDUCE_TYPE,FLOAT_TYPE)]     =    profile_base_ptr(new scalar_reduction("float",4,64,512,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,FLOAT_TYPE)]  =    profile_base_ptr(new vector_reduction("float",'T',1,1,256,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,FLOAT_TYPE)]  =    profile_base_ptr(new vector_reduction("float",'N',1,64,4,64));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','T',1,16,32,16,4,1,4,1,1,32,8));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','T',1,8,16,16,8,2,4,1,1,32,4));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','N',1,8,16,16,4,1,8,1,1,16,8));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','N',1,8,16,16,4,1,8,1,1,16,8));

          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_SAXPY_TYPE,DOUBLE_TYPE)]      =    profile_base_ptr(new vector_saxpy("double",2,1,64,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_SAXPY_TYPE,DOUBLE_TYPE)]      =    profile_base_ptr(new matrix_saxpy("double",2,16,16,16,16,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(SCALAR_REDUCE_TYPE,DOUBLE_TYPE)]     =    profile_base_ptr(new scalar_reduction("double",2,64,512,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,DOUBLE_TYPE)]  =    profile_base_ptr(new vector_reduction("double",'T',1,1,128,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,DOUBLE_TYPE)]  =    profile_base_ptr(new vector_reduction("double",'N',1,16,32,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','T',1,8,16,16,8,1,4,1,1,32,4));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','T',1,8,16,16,8,1,4,1,1,32,4));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','N',1,4,128,32,4,8,4,0,0,0,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Fermi]["GeForce GTX 470"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','N',1,4,32,16,8,4,8,0,0,0,0));

          //default
          set_all_generation_default_to(map,nvidia_id,Fermi,"GeForce GTX 470");

          //-----Kepler

          //Geforce GTX 680 (based on feedback from Paul Dufort via viennacl-support)
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_SAXPY_TYPE,FLOAT_TYPE)]      =    profile_base_ptr(new vector_saxpy("float",1,1,256,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_SAXPY_TYPE,FLOAT_TYPE)]      =    profile_base_ptr(new matrix_saxpy("float",1,16,16,16,16,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(SCALAR_REDUCE_TYPE,FLOAT_TYPE)]     =    profile_base_ptr(new scalar_reduction("float",4,64,512,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,FLOAT_TYPE)]  =    profile_base_ptr(new vector_reduction("float",'T',1,1,256,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,FLOAT_TYPE)]  =    profile_base_ptr(new vector_reduction("float",'N',1,64,4,64));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','T',1,2,64,64,8,4,2,0,0,0,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','T',1,8,16,16,8,1,4,1,1,32,4));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'T','N',1,2,32,32,8,4,2,0,0,0,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,FLOAT_TYPE)] = profile_base_ptr(new matrix_product("float",'N','N',1,32,16,8,4,8,4,0,0,0,0));

          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_SAXPY_TYPE,DOUBLE_TYPE)]      =    profile_base_ptr(new vector_saxpy("double",2,1,64,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_SAXPY_TYPE,DOUBLE_TYPE)]      =    profile_base_ptr(new matrix_saxpy("double",2,16,16,16,16,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(SCALAR_REDUCE_TYPE,DOUBLE_TYPE)]     =    profile_base_ptr(new scalar_reduction("double",2,64,512,true));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_REDUCE_Tx_TYPE,DOUBLE_TYPE)]  =    profile_base_ptr(new vector_reduction("double",'T',1,1,128,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(VECTOR_REDUCE_Nx_TYPE,DOUBLE_TYPE)]  =    profile_base_ptr(new vector_reduction("double",'N',1,16,32,1024));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_TT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','T',1,8,64,32,2,2,8,0,0,0,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_NT_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','T',1,8,16,16,8,1,4,1,1,32,4));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_TN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'T','N',1,4,128,32,4,8,4,0,0,0,0));
          map[nvidia_id][CL_DEVICE_TYPE_GPU][Kepler]["GeForce GTX 680"][std::make_pair(MATRIX_PRODUCT_NN_TYPE,DOUBLE_TYPE)] = profile_base_ptr(new matrix_product("double",'N','N',1,4,32,16,8,4,8,0,0,0,0));

          //default
          set_all_generation_default_to(map,nvidia_id,Kepler,"GeForce GTX 680");


        return map;
      }
      static database_type database = init_database();

      /** @brief If the fallback is too harsh, use a very conservative profile */
      static profile_base & handle_failure(viennacl::ocl::device const & device, expression_key_type const & key, tools::shared_ptr<profile_base> const & profile){
        //Returns default if the profile is invalid
        if(profile->is_invalid(device))
          return *database.map.at(unknown_id).map.at(device.type()).map.at(UNKNOWN).map.at("").map.at(key).get();
        return *profile.get();
      }

      /** @brief Get the profile for a device and a descriptor */
      static profile_base & get(expression_type expression, expression_numeric_type numeric_type){
        viennacl::ocl::device const & device = viennacl::ocl::current_device();

        device_type dev_type = device.type();
        vendor_id_type vendor_id = device.vendor_id();
        device_architecture_family device_architecture = device.architecture_family();
        std::string const & device_name = device.name();
        expression_key_type expression_key(expression, numeric_type);

        //std::cout << "Looking up vendor ID..." << std::endl;
        /*-Vendor ID-*/
        database_type::map_type::iterator vendor_it = database.map.find(vendor_id);
        //Vendor not recognized => global default:
        if(vendor_it==database.map.end())
          return handle_failure(device, expression_key, database.map.at(unknown_id).map.at(dev_type).map.at(UNKNOWN).map.at("").map.at(expression_key));

        /*-Device Type-*/
        //std::cout << "Looking up device type..." << std::endl;
        device_type_map::map_type::iterator device_type_it = vendor_it->second.map.find(dev_type);
        //Device type not recognized for this vendor => global default
        if(device_type_it==vendor_it->second.map.end())
          return handle_failure(device, expression_key, database.map.at(unknown_id).map.at(dev_type).map.at(UNKNOWN).map.at("").map.at(expression_key));

        /*-Device Architecture-*/
        //std::cout << "Looking up device architecture..." << std::endl;
        device_architecture_map::map_type::iterator architecture_it = device_type_it->second.map.find(device_architecture);
        if(architecture_it==device_type_it->second.map.end())
          return handle_failure(device, expression_key, database.map.at(unknown_id).map.at(dev_type).map.at(UNKNOWN).map.at("").map.at(expression_key));

        /*-Device Name-*/
        //std::cout << "Looking up device name..." << std::endl;
        device_name_map::map_type::iterator device_name_it = architecture_it->second.map.find(device_name);
        //Name not found => Vendor default
        if(device_name_it==architecture_it->second.map.end())
          return handle_failure(device, expression_key, database.map.at(vendor_id).map.at(dev_type).map.at(device_architecture).map.at("").map.at(expression_key));

        //std::cout << "Looking up expression name.." << std::endl;
        /*-Expression-*/
        expression_map::map_type::iterator expression_it = device_name_it->second.map.find(expression_key);
        //Expression not found => Vendor default
        if(expression_it==device_name_it->second.map.end())
          return handle_failure(device, expression_key, database.map.at(vendor_id).map.at(dev_type).map.at(device_architecture).map.at("").map.at(expression_key));

        //std::cout << "Device found in the database! Getting profile..." << std::endl;
        //Everything okay. Return specific profile//
        return handle_failure(device, expression_key, database.map.at(vendor_id).map.at(dev_type).map.at(device_architecture).map.at(device_name).map.at(expression_key));
      }

    }

  }

}


#endif
