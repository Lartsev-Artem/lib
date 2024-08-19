set (exclude_list "build")

#����� �������������
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child} AND NOT "${child}" STREQUAL "Eigen")
      LIST(APPEND dirlist ${child})
      ENDIF()   
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

#��������� ������� �� 3 �������
MACRO(INC_SUBDIR  curdir) 
    list(APPEND subdirs_list "${curdir}")        
    SUBDIRLIST(SUBDIRS ${curdir})
    FOREACH(child ${SUBDIRS})        
        list(APPEND subdirs_list "${curdir}/${child}")            
        SUBDIRLIST(SUBDIRS2 ${curdir}/${child})
        FOREACH(child2 ${SUBDIRS2})            
             list(APPEND subdirs_list "${curdir}/${child}/${child2}")    
            SUBDIRLIST(SUBDIRS3 ${curdir}/${child}/${child2})
            FOREACH(child3 ${SUBDIRS3})               
                list(APPEND subdirs_list "${curdir}/${child}/${child2}/${child3}")                    
            ENDFOREACH()
        ENDFOREACH()        
    ENDFOREACH()
ENDMACRO()


#����������� � ������� ���������� � ������������ ������ ��������-������
MACRO(INCLUDE_SRC_IN_PRJ SOURCES_DIR src_files) 
# ������� ���� ��������� � �����������
    FOREACH(dir_src ${SOURCES_DIR})
        INC_SUBDIR(${dir_src})
    ENDFOREACH()

    set(src_files "")
    FOREACH(dir ${subdirs_list})
        include_directories("${dir}")   #��������� ���� ������������
        list(APPEND src_files "${dir}/*.cpp")        #���������� ���� �������� ������
        # message(WARNING ${dir})
    ENDFOREACH()  
ENDMACRO()


function(get_all_files mask filelist)

	file(GLOB_RECURSE file_list ${mask})
#ifndef CRYPTO_MODEL_UNIFY
	#message("Hdr list:" ${file_list})
	if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_LESS "3.6")
		foreach(fn ${file_list})		
			foreach(del ${exclude_list})				
				string(FIND ${fn} "${del}" fn_remove)
				if(NOT "${fn_remove}" STREQUAL "-1")
					list(REMOVE_ITEM file_list ${fn})					
				endif()
			endforeach()
		endforeach()
	else()
	foreach(del ${exclude_list})	
		list(FILTER file_list EXCLUDE REGEX ".+/${del}")
	endforeach()
	endif()
	#message("Hdr list no _na_:" ${file_list})
#endif
        set(${filelist} "${file_list}" PARENT_SCOPE)
endfunction(get_all_files)

function(get_dirs_from_files files dirs)	
	set(dir_list "")
	foreach(file_path ${files})
	   if (NOT ${file_path} EQUAL "")
	   	get_filename_component(dir_path ${file_path} PATH)
	   	set(dir_list ${dir_list} ${dir_path})
	   endif()
	endforeach()
	list(REMOVE_DUPLICATES dir_list)
	set(${dirs} ${dir_list} PARENT_SCOPE)
endfunction(get_dirs_from_files)