cmake_minimum_required(VERSION 3.10)

project(arxProject)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h) 
file(GLOB SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.rc)

set(ALL_RSC_FILES ${HEADERS} ${SOURCE} ${RESOURCE})
add_library(arxProject SHARED ${ALL_RSC_FILES})
#设置目标拓展名
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".arx")

set(ARX2022DIR $ENV{ARX2022DIR})
#link_directories(${ARX_LIB_DIR})	#链接路径（无效且cmake不推荐使用，因为通过全路径链接库可以让cmake查找是否存在）
target_link_libraries(${PROJECT_NAME} 
	${ARX2022DIR}/lib-x64/accore.lib 
	${ARX2022DIR}/lib-x64/acad.lib
	${ARX2022DIR}/lib-x64/ac1st24.lib
	${ARX2022DIR}/lib-x64/acdb24.lib
	${ARX2022DIR}/lib-x64/acge24.lib
	${ARX2022DIR}/lib-x64/acgiapi.lib
	${ARX2022DIR}/lib-x64/acISMobj24.lib
	${ARX2022DIR}/lib-x64/rxapi.lib
	${ARX2022DIR}/lib-x64/axdb.lib
	${ARX2022DIR}/lib-x64/acdbmgd.lib
	${ARX2022DIR}/lib-x64/AcPal.lib
	${ARX2022DIR}/lib-x64/AcFdEval.lib
	)
set(ARX_INC_DIRS ${ARX2022DIR}/inc ${ARX2022DIR}/inc-x64)
include_directories(${ARX_INC_DIRS})

add_definitions(-D_ACRXAPP -D_NDEBUG=1 -D_WIN64_IE=0x0601 -DWIN -DWIN32
-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1 
-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT=1
-D_AFXDLL 
-D_UNICODE 
-DUNICODE 
-D_WINDLL
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	