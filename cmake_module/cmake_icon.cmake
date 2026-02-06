
function(configure_icon_resources TARGET_PATH ICON_SRC)
    if(NOT WIN32)
        return()
    endif()

    get_filename_component(ICON_NAME ${ICON_SRC} NAME)

    # 1. Копируем иконку в бинарную директорию
    configure_file(
        ${ICON_SRC}
        ${TARGET_PATH}/${ICON_NAME}
        COPYONLY
    )

message(WARNING NAME: ${ICON_NAME})

    # 2. Создаем простой .qrc файл
    file(WRITE ${TARGET_PATH}/app_resources.qrc
    "<!DOCTYPE RCC><RCC version=\"1.0\">
    <qresource prefix=\"/\">
        <file>${ICON_NAME}</file>
    </qresource>
    </RCC>"
    )

endfunction()

function(configure_windows_exe_icon TARGET_PATH ICON_SRC)
    if(NOT WIN32)
        return()
    endif()

    get_filename_component(ICON_NAME ${ICON_SRC} NAME)

    # 1. Копируем иконку в бинарную директорию
    configure_file(
        ${ICON_SRC}
        ${TARGET_PATH}/${ICON_NAME}
        COPYONLY
    )

    file(WRITE ${TARGET_PATH}/win_resources.rc
        "IDI_ICON1 ICON \"${ICON_NAME}\"\n"
    )
    target_sources(${PROJECT_NAME} PRIVATE ${TARGET_PATH}/win_resources.rc)
endfunction()
