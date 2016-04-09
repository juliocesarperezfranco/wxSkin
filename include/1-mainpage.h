/** 	\author René Kraus aka upCASE
	\version 0.9

	\mainpage
	
	\section whatis_skin What is wxSkin?
	wxSkin is a set of skinable controls (like buttons, checkboxes etc.) for the wxWidgets framework. "Skins" are sets of images which are used to create a GUI instead of using a 
	standard interface. Therefor, when using wxSkins control, a GUI can be customized in many places, like we know most media players do. wxSkin tries to bridge the developer <-> user gap.
	It offers a simple API for developers who want to be able to have their applications (or part of it) "skinned". Users may change the look&feel of the applications by changing to skins that suit
	their needs. As the "skin" format is the same for all wxSkin-based applications it is very simple for designers to create new skins.
	
	Here's a small example screenshot of wxSkin in action on Windows.
	\image html wxskin2.jpg
	
	And here the same application and skin on my Gnome Desktop.
	\image html skingtk2.jpg
	
	And again running on Mac OS X Tiger
	\image html skin1osx.jpg
	
	\subsection skin_features Some wxSkin features
	- Offers a set of skinnable controls
	- Skins can be created using a simple XML format file
	- Skins can be loaded through XML files and will be assigned to the current controls
	- Skins can either be located a simple files on disk, or in zipped form as a self-contained skin file
	- Skins can be changed during application runtime without the need to restart
	- wxSkin offers support for including GUIBuilder scripts to create/change the layout of the GUI
	- wxSkin is cross-platform: One skin on many platforms! Currently supports Windows, GTK and Mac OSX.

	\section why_skin Why wxSkin?
	wxWidgets is a great cross-platform GUI framework, but it lacks the ability to easily create custum GUIs. Although wxWidgets primary goal is to use native controls where it is possible,
	there has been a demand in the community to have a simple way of creating skinned applications or customly shaped controls.
	
	wxSkin does not mean to replace anything in wxWidgets, but to offer a simple addon-API on top of wxWidgets to create customly shaped controls and applications.
	
	The difference between wxSkin and other projects, like wxUniversal, is, that wxSkin does not create GUIs using "renderers" (code snippets creating parts of a GUI), but image files. "Renderers" are not
	editable by users or designers unless they start coding. wxSkin offer a simple file format in XML that allows designer/users to create their own skins.
	
	Although it might have been possible to create wxSkin as a "renderer" for wxUniversal, I decided to create a new set of classes that can be used independently.
	
	\section wxskin_how How does wxSkin work?
	As I already said wxSkin comes as a set of new controls, each with the abiltiy to be skinned. Skin images can have any format that is supported by wxWidgets.
	
	Skins can have transparent sections that will be used to create the GUI elements shaped. The masks needed to shape a skin can either be created by specifying a mask colour for an image, or, if the image
	format supports it, by using an alpha channel. If wxSkin finds an alpha channel it will try to convert this to a mask for later usage.
	
	Although it is possible to create each skinned GUI control seperatly and pass it the images it would need to be properly created, wxSkin offers a much simpler way for controling the skinning process.
	wxSkin features a class called wxSkinEngine, which is able to load and parse XML files. In these XML files you can specify the images to use for different controls, name these controls and pass additional
	options, like position and size. Through wxSkinEngine you can assign GUI controls windows and associate them with label names. If wxSkinEngine was able to parse the XML files and setup the associations,
	all you have to do is call a method that will intialize the new skin and recreate the whole GUI accordingly.
	
	Skin files that can be processed by wxSkinEngine come in two falvors: As simple XML files containing the location of images to use, or as a self-contained zip archive. Normaly one would create the XML file
	and the skin images, place them in a directory with a structure of choice and then simply zip it up. The generated archive only has to follow three rules:
	-# The XML file will have to be named skin.xml and located in the root of the archive.
	-# When using an archive you can't reference images outside the archive.
	-# The archive has to be in ZIP format.
	
	Skin files can contain optional informations, like author, comment and screenshot. Through wxSkinEngine this informations a easily accessible. For convenience there is wxSkinChooser, a simple frame that
	scans a directy hierarchy for files specified by a file mask and tries to get the info out of these files. It then creates a simple list of all found skin files with screenshots. When double-clicking a skin,
	wxSkinChooser will load the file and change the GUI skins.
	
	\subsection skin_work Working wxSkin Controls
	- wxSkinButton
	- wxSkinRadiobutton
	- wxSkinCheckBox
	- wxSkinGauge
	- wxSkinSlider
	- wxSkinPanel
	- wxSkinStaticText
	- wxSkinSimpleFrame
	- wxSkinFrame
	
	\todo Although wxSkin works properly in most cases, there still is some bug testing to do. Some things are still in a "planning" state, others might not work as expected.
	- Support "proper" resizing for "complex" frames on all platforms (currently GTK is a bit buggy)
	- Sliders need some improvement regarding skinning and handling
	- Finish the docs
	- Add font loading support
	- Add the ability to use GIF animations (e.g. for fancy moving progressbars)
	- Add a "knob" control that can be controled with the mouse
	
	I hope you like the results of my work so far. I'm open for feedback, questions, comments etc. You can contact me via the wxForum at http://wxforum.shadonet.com.
	
	- \ref skin_file - explains the skin file format
	- \ref skin_tutorial - have a look at the small tutorial I created.
	
	Have fun with wxSkin!
	
     \image html wxskin1.jpg
  
    
*/