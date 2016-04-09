
## Author:      René Kraus
## Copyright:   (c) René Kraus
## Credits:     Based on initial work by Jorgen Bodde
## License:     wxWidgets License
##---------------------------------------------------------------------------

##---------------------------------------------------
## Please set your wxWidgets configuration here
##--------------------------------------------------- 
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0043 OLD)
ENDIF(COMMAND CMAKE_POLICY)

IF(NOT SKIP_FIRSTTIME_MESSAGE)
	MESSAGE("Welcome!\nPlease be reminded that by default a release build of wxSkin will be configured.\nThe simple and complex example will be build as well.\nTo enable the full media player example you will have to enable WXSKIN_MEDIACTRL_DEMO and have wxMediaCtrl compiled.")
	SET(SKIP_FIRSTTIME_MESSAGE ON CACHE INTERNAL BOOL)
ENDIF(NOT SKIP_FIRSTTIME_MESSAGE)

OPTION(WXSKIN_EXAMPLE  "Compile a simple wxSkin example application" OFF)
OPTION(WXSKIN_COMPLEX_EXAMPLE "Compile an example application demonstrating complex frames" OFF)
OPTION(WXSKIN_MEDIACTRL_DEMO "Compile full mediaplayer example (needs wxMediaCtrl)" OFF)


SET(WXW_USE_DEBUG OFF)
SET(WXW_USE_UNICODE OFF)
SET(WXW_USE_SHARED OFF)
SET(WXW_USE_UNIV OFF)
SET(WXW_USE_MONO OFF)

SET(WXW_FILE_VERSION "30")
SET(WXW_VERSION "3.0")

PROJECT( WXSKIN )

## Find wxWidgets
FIND_PACKAGE(wxWidgets COMPONENTS core base xml adv aui richtext REQUIRED)
INCLUDE(${wxWidgets_USE_FILE})

ADD_DEFINITIONS( ${WXWIDGETS_DEFINITIONS} )

CONFIGURE_FILE(${WXSKIN_SOURCE_DIR}/skinfiles/skin.zip
               ${WXSKIN_BINARY_DIR}/skinfiles/skin.zip COPYONLY)
CONFIGURE_FILE(${WXSKIN_SOURCE_DIR}/skinfiles/skin2.zip
               ${WXSKIN_BINARY_DIR}/skinfiles/skin2.zip COPYONLY)
CONFIGURE_FILE(${WXSKIN_SOURCE_DIR}/skinfiles/skin3.zip
               ${WXSKIN_BINARY_DIR}/skinfiles/skin3.zip COPYONLY)

CONFIGURE_FILE(${WXSKIN_SOURCE_DIR}/skinfiles/skin_complex.zip
               ${WXSKIN_BINARY_DIR}/skinfiles/skin_complex.zip COPYONLY)
CONFIGURE_FILE(${WXSKIN_SOURCE_DIR}/example/example.jpg
               ${WXSKIN_BINARY_DIR}/example.jpg COPYONLY)
	       
CONFIGURE_FILE(${WXSKIN_SOURCE_DIR}/example/skin_template.xml
               ${WXSKIN_BINARY_DIR}/template/skin_template.xml COPYONLY)

SET(LIB_SRCS src/wxSkinButton.cpp
         include/wxSkinButton.h
	 src/wxSkinCheckBox.cpp
	 include/wxSkinCheckBox.h
	 src/wxSkinChooser.cpp
	 include/wxSkinChooser.h
	 src/wxSkinEngine.cpp
	 include/wxSkinEngine.h
	 src/wxSkinFrame.cpp
	 include/wxSkinFrame.h
	 src/wxSkinFrameBase.cpp
	 include/wxSkinFrameBase.h
	 src/wxSkinGauge.cpp
	 include/wxSkinGauge.h
	 src/wxSkinPanel.cpp
	 include/wxSkinPanel.h
	 src/wxSkinRadiobutton.cpp
	 include/wxSkinRadiobutton.h
	 src/wxSkinSimpleFrame.cpp
	 include/wxSkinSimpleFrame.h
	 src/wxSkinSlider.cpp
	 include/wxSkinSlider.h
	 src/wxSkinStaticText.cpp
	 include/wxSkinStaticText.h
	 src/wxSkinWindow.cpp
	 include/wxSkinWindow.h
	 src/guibuilder.cpp
	 include/guibuilder.h
	 src/wxSkinSplitterWindow.cpp
	 include/wxSkinSplitterWindow.h
          )

SET(SRCS_NORMAL example/main.cpp)
SET(SRCS_COMPLEX example/main_complex.cpp)
SET(SRCS_FULL example/main_full.cpp)

IF(UNIX AND NOT APPLE)
	ADD_DEFINITIONS( -DLINUX=1 )
	PKGCONFIG("gtk+-2.0")
	IF(PKGCONFIG_FOUND)
		IF(CMAKE_PKGCONFIG_C_FLAGS)
			SET(CMAKE_CXX_FLAGS "${CMAKE_PKGCONFIG_C_FLAGS} ${CMAKE_CXX_FLAGS}")
			SET(WXWIDGETS_LIBRARIES "${PKGCONFIG_LIBRARIES} ${WXWIDGETS_LIBRARIES}")
			#do something with ${PKGCONFIG_LIBRARIES}
		ENDIF(CMAKE_PKGCONFIG_C_FLAGS)
		ELSE(PKGCONFIG_FOUND)
			MESSAGE("Cannot find gtk+-2.0")
	ENDIF(PKGCONFIG_FOUND)
ENDIF(UNIX AND NOT APPLE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}${WXWIDGETS_CXX_FLAGS}")


INCLUDE_DIRECTORIES(${GUIBUILDER_SOURCE_DIR}
                    ${WXSKIN_SOURCE_DIR}/include
                    ${wxWidgets_INCLUDE_DIR}
                  )

LINK_DIRECTORIES( ${WXSKIN_BINARY_DIR}
                  ${wxWidgets_LINK_DIRECTORIES}
                  ${LINK_DIRECTORIES} )

#Mac stuff
IF(APPLE)
	SET(MACOSX_BUNDLE_INFO_STRING "wxSkin")
	SET(MACOSX_BUNDLE_ICON_FILE "wxmac.icns")
	SET(MACOSX_BUNDLE_BUNDLE_NAME "wxSkin")
	SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "0.9")
	SET(MACOSX_BUNDLE_COPYRIGHT "2006 upCASE")
ENDIF(APPLE)

ADD_LIBRARY(wxSkin ${LIB_SRCS})

IF(WXSKIN_EXAMPLE)
	ADD_EXECUTABLE(wxSkin_example WIN32 MACOSX_BUNDLE ${SRCS_NORMAL})
	TARGET_LINK_LIBRARIES(wxSkin_example wxSkin ${wxWidgets_LIBRARIES})
ENDIF(WXSKIN_EXAMPLE)

IF(WXSKIN_COMPLEX_EXAMPLE)
	ADD_EXECUTABLE(wxSkin_example_complex WIN32 MACOSX_BUNDLE ${SRCS_COMPLEX})
	TARGET_LINK_LIBRARIES(wxSkin_example_complex wxSkin ${wxWidgets_LIBRARIES})
ENDIF(WXSKIN_COMPLEX_EXAMPLE)

IF(WXSKIN_MEDIACTRL_DEMO)
	#trigger wxWidget script again to get media libs
	SET(WXWIDGETS_USE_MEDIA ON)
	INCLUDE (${wxWidgets_USE_FILE})
	
	ADD_EXECUTABLE(wxSkin_example_full WIN32 MACOSX_BUNDLE ${SRCS_FULL})
	IF(WIN32 AND NOT MINGW)
		TARGET_LINK_LIBRARIES(wxSkin_example_full wxSkin ${wxWidgets_LIBRARIES} Winmm.lib)
	ELSE(WIN32 AND NOT MINGW)
		TARGET_LINK_LIBRARIES(wxSkin_example_full wxSkin ${wxWidgets_LIBRARIES})
	ENDIF(WIN32 AND NOT MINGW)
ENDIF(WXSKIN_MEDIACTRL_DEMO)
