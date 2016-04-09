/**	
	\page skin_file wxSkin XML file format

	This page covers all elements of the XML format valid for creating wxSkin files.
	
	\htmlonly
<style type="text/css">
.S0 {
	font-family: 'Times New Roman';
	color: #000000;
	background: #FFFFFF;
	font-size: 11pt;
}
.S1 {
	color: #000080;
	background: #FFFFFF;
}
.S3 {
	color: #008080;
	background: #FFFFFF;
}
.S6 {
	color: #7F007F;
	background: #FFFFFF;
}
.S8 {
	color: #800080;
	background: #FFFFFF;
}
.S11 {
	color: #000080;
	background: #FFFFFF;
}
.S12 {
	font-weight: bold;
	color: #800080;
	background: #FFFFFF;
}
.S13 {
	font-weight: bold;
	color: #800080;
	background: #FFFFFF;
}

</style>
	\endhtmlonly

	Each wxSkin XML file starts, as every other XML file, with the xml tag:
	
\htmlonly
<ul>
<span><span class="S12">&lt;?</span><span class="S1">xml</span><span class="S8"> </span><span class="S3">version</span><span class="S8">=</span><span class="S6">"1.0"</span><span class="S8"> </span><span class="S3">encoding</span><span class="S8">=</span><span class="S6">"ISO-8859-1"</span><span class="S13">?&gt;</span></span>
</ul>
\endhtmlonly

	\section xml_start The Skinfile body
	
	The root tag, which contains all XML skin definitions for wxSkin files, will have to be named "Skin" and should look like:
	
\htmlonly	
<ul>
<span><span class="S1">&lt;Skin</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">"Example skin"</span><span class="S8"> </span><span class="S3">author</span><span class="S8">=</span><span class="S6">"Rene aka upCASE"</span><br />
<span class="S8">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S3">comment</span><span class="S8">=</span><span class="S6">"A simple mediaplayer skin\nCredits go to jix for creating the VLC skin"</span><span class="S8"> </span><br />
<span class="S8">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S3">screenshot</span><span class="S8">=</span><span class="S6">"wxskin2.jpg"</span><span class="S8"> </span><span class="S3">application</span><span class="S8">=</span><span class="S6">"wxSkin Media Player example"</span><br />
<span class="S8">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S3">customInfo</span><span class="S8">=</span><span class="S6">""</span><span class="S1">&gt;</span><br />
<span class="S1">&lt;/Skin&gt;</span></span>
</ul>
\endhtmlonly

	As you can see, the "Skin" tag can have some property values, namely:
	- name: 	Specifies a name for this skin
	- author: 	The author/creator of the skin
	- comment:	Some comment on this skin file, like credits etc.
	- screenshot:	The location of a screenshot of this skin in use. Note that screenshots should not be bigger than 300*300 px in size. Otherwise it will be scaled down.
	- application:	A string specifying the application this skin is meant for
	- customInfo:	A field for custom informations your application might need.
	
	Note that you do not have to provide any of these properties, but it's adviced that you at least use the "name" property.
	
	\section xml_transparency Definitions for transparency
	The "transparency" tag can occur once in every skin file.
\htmlonly
<ul>
<span class="S1">&lt;Transparency</span><span class="S8"> </span><span class="S3">red</span><span class="S8">=</span><span class="S6">"255"</span><span class="S8"> </span><span class="S3">green</span><span class="S8">=</span><span class="S6">"0"</span><span class="S8"> </span><span class="S3">blue</span><span class="S8">=</span><span class="S6">"255"</span><span class="S8"> </span><span class="S11">/&gt;</span>
</ul>
\endhtmlonly
	This tag specifies the RGB values that will be used for creating transparency masks if a skin image doesn't already have transparent areas. The standard RGB values if this isn't specifed are 255,0,255.
	
	\section xml_simple_frame Definitions for wxSkinSimpleFrame
	Now you may want to create a frame. First, let's go with a wxSkinSimpleFrame definition:
	
\htmlonly	
<ul>
<span><span class="S1">&lt;SimpleFrame</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">"PlayerFrame"</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"fond.png"</span><span class="S8"> </span><span class="S3">movable</span><span class="S8">=</span><span class="S6">"yes"</span><span class="S8"> &nbsp;</span><span class="S3">guibuilder</span><span class="S8">=</span><span class="S6">"something.txt"</span><span class="S1">&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;client_area</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"165,35,409,230"</span><span class="S8"> </span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;closebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"680,219,-1,-1"</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"close.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"close_over.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;minimizebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"660,252,-1,-1"</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"reduce.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"reduce_over.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;maximizebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"2,2,2,2"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titlebar</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"160,10,418,20"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titleposition</span><span class="S8"> </span><span class="S3">placement</span><span class="S8">=</span><span class="S6">"middle"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titlefont</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">"Comic Sans MS"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"12"</span><span class="S8"> </span><span class="S3">weight</span><span class="S8">=</span><span class="S6">"normal"</span><span class="S8"> </span><span class="S3">underline</span><span class="S8">=</span><span class="S6">"false"</span><span class="S8"> </span><span class="S3">italic</span><span class="S8">=</span><span class="S6">"false"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;/SimpleFrame&gt;</span></span>
</ul>
\endhtmlonly

	The valid properties for "SimpleFrame" are:
	- name: The name of the frame. This label will be used for later assignments of the definition to a wxSkinSimpleFrame instance.
	- skin: Specifies the skin file to use. This can be any image wxWidgets is able to process. It may contain transparent areas.
	- movable: Can either be "yes" or "no". "Yes" means that the frame is movable when the user clicks and drags it anywhere in the area of the skin image, not only the titlebar.
	- guibuidler: The location of a guibuilder script. For a reference on guibuilder, check Jorgen Boddes site (http://wastebucket.solidsteel.nl/cms/modules/wiwimod/index.php?page=wxGuiBuilder&back=WiwiHome).
	Note that the version wxSkin uses is a bit different compared to the "normal" version. You can use "labels" in a guibuilder script to control the layout of assigned controls.
	
	Enclosed in the "SimpleFrame" tag are further options for the frame.
	
	\htmlonly
	<ul>
	<span class="S1">&lt;client_area</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"165,35,409,230"</span><span class="S8"> </span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	The client_area specifies that there is an area in the frame that will be used for adding a sizer that contains other elements. When you call SetSizer() on a wxSkinSimpleFrame, the sizer will be placed inside this area.
	"measure" give the location of this area in the form "x,y,width,height". The location is the abosulte location in the image.
	
	\htmlonly
	<ul>
	<span class="S1">&lt;closebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"680,219,-1,-1"</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"close.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"close_over.png"</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	"closebox" is an area that will close the frame when the users clicks inside it. The "measure" property follows the same rules as for "client_area". The "normal" and "over" properties
	are locations of images for "normal" and "over" state. You don't have to use the images in order to define an area to close the frame.
	\htmlonly
	<ul>
	<span class="S1">&lt;maximizebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"2,2,2,2"</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	See "closebox".
	\htmlonly
	<ul>
	<span class="S1">&lt;minimizebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"660,252,-1,-1"</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"reduce.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"reduce_over.png"</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	See "closebox".
	\htmlonly
	<ul>
	<span class="S1">&lt;titlebar</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"160,10,418,20"</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	"titlebar" is an area that will be used as the titlebar or caption. If you pass a title for the frame it will be located inside this area.
	\htmlonly
	<ul>
	<span class="S1">&lt;titleposition</span><span class="S8"> </span><span class="S3">placement</span><span class="S8">=</span><span class="S6">"middle"</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	"titleposition" specifies where the title is placed inside the titlebar. "placement" can either be "left, "middle" or "right". The title will be placed accordingly.
	\htmlonly
	<ul>
	<span class="S1">&lt;titlefont</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">"Comic Sans MS"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"12"</span><span class="S8"> </span><span class="S3">weight</span><span class="S8">=</span><span class="S6">"normal"</span><span class="S8"> </span><span class="S3">underline</span><span class="S8">=</span><span class="S6">"false"</span><span class="S8"> </span><span class="S3">italic</span><span class="S8">=</span><span class="S6">"false"</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	"titlefont"  specifies a font to use in the titlebar. The font has to be installed on the system. Later on wxSkin may add a feature to load these at runtime.
	- name: The facename of the font. This is not a filename.
	- size: The pointsize of the font.
	- weight: Can either be "normal", "light" or "bold".
	- underline: Either "true" or "false". Default is false.
	- italic: Either "true" or "false". Default is false.
	
	There can be multiple wxSkinSimpleFrame definitions on one skin file.
	
	\section xml_frame Definitions for wxSkinFrame
	wxSkinFrame has some similarities with wxSkinSimpleFrame, yet it is more "complex" as the GUI is created by more then just one image and it has some more features.
	In general wxSkinFrame behaves like wxSkinSimpleFrame, but it tries to mimic standard frames, meaning that the frame can be resized and most likely has sort of a recangular shape.
	To create a wxSkinFrame you'll need 9 images, 4 for the borders, 4 for window "edges" and 1 for the "body" (client) area. Let's have a look at the definition:
	\htmlonly
<ul>
<span><span class="S1">&lt;Frame</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">"ComplexFrame"</span><span class="S8"> </span><span class="S3">movable</span><span class="S8">=</span><span class="S6">"yes"</span><span class="S8"> </span><span class="S3">guibuilder</span><span class="S8">=</span><span class="S6">"layoutscript.txt"</span><span class="S1">&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;top</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"top.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;top_left</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"top_left.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;top_right</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"top_right.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;bottom</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"bottom.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;bottom_left</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"bottom_left.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;bottom_right</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"bottom_right.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;left</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"left.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;right</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"right.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;body</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"body.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;closebox</span><span class="S8"> </span><span class="S3">relate_to</span><span class="S8">=</span><span class="S6">"top"</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"657,4,-1,-1"</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"close.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"close_over.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;minimizebox</span><span class="S8"> </span><span class="S3">relate_to</span><span class="S8">=</span><span class="S6">"top"</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"640,4,-1,-1"</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"minimize.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"minimize_over.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;maximizebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"0,0,0,0"</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titlebar</span><span class="S8"> </span><span class="S3">height</span><span class="S8">=</span><span class="S6">"25"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titleposition</span><span class="S8"> </span><span class="S3">placement</span><span class="S8">=</span><span class="S6">"middle"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titlefont</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">"Comic Sans MS"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"12"</span><span class="S8"> </span><span class="S3">weight</span><span class="S8">=</span><span class="S6">"normal"</span><span class="S8"> </span><span class="S3">underline</span><span class="S8">=</span><span class="S6">"false"</span><span class="S8"> </span><span class="S3">italic</span><span class="S8">=</span><span class="S6">"false"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;/Frame&gt;</span></span>
</ul>
	\endhtmlonly
	The valid properties for a Frame are:
	- name: The name of the frame. This label will be used for later assignments of the definition to a wxSkinFrame instance.
	- movable: Can either be "yes" or "no". "Yes" means that the frame is movable when the user clicks and drags it anywhere in the area of the skin image, not only the titlebar.
	- guibuidler: The location of a guibuilder script. For a reference on guibuilder, check Jorgen Boddes site (http://wastebucket.solidsteel.nl/cms/modules/wiwimod/index.php?page=wxGuiBuilder&back=WiwiHome).
	Note that the version wxSkin uses is a bit different compared to the "normal" version. You can use "labels" in a guibuilder script to control the layout of assigned controls.
	
	You'll have to set some more entities for proper creation of a wxSkinFrame.
	\htmlonly
	<ul>
<span class="S1">&lt;top</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"top.png"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;top_left</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"top_left.png"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;top_right</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"top_right.png"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;bottom</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"bottom.png"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;bottom_left</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"bottom_left.png"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;bottom_right</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"bottom_right.png"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;left</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"left.png"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;body</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"body.png"</span><span class="S11">/&gt;</span><br />
	</ul>
	\endhtmlonly
	These entities control the skin loading. "skin" is always the location of a valid image file wxWidgets can process. Again, skin images may have transparency.
	- top, bottom, left and right set the skins for the "borders"
	- top_left, top_right, bottom_left and bottom_right set the skins for the frames "edges"
	- body sets the skin for the client area
	
	The creation of a GUI for a "complex" frame is, well, a bit complex :-)
	
	The GUI will be created by placing the "edges" in the correct location. The "borders" will be sized according to the frames size and will be resized when a user resizes the frame. The body area 
	will be reiszed, too. In addition to that, the body area is used when you create a sizer for the frame. The sizer will be set so that it fits inside the body area. Note that you should *not* use a 
	transparent skin for the body. The problem is that on some platforms (namely MSW) when using a transparent body, widgets that you place on that area will have the same transparency. Unless
	you want that for some really fancy frame, use a solid skin.
	
	Let's have a look at the other options:
	\htmlonly
	<ul>
	<span class="S1">&lt;closebox</span><span class="S8"> </span><span class="S3">relate_to</span><span class="S8">=</span><span class="S6">"top"</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"657,4,-1,-1"</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"close.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"close_over.png"</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	closebox specifies a cloase box for the frame. It may have the following properties:
	- relate_to: controls the "relation" of the closebox. This can currently be "top" or "absolute". "top" means that when resizing the frame, the closebox is moved as well. "absolute" means that the closebox will never be moved. Additional "relations" may be added in the future.
	- measure: controls the initial placement and size. "measure" has the form "x,y,width,height". 
	- normal: sets a skin for the normal state of the closebox
	- over: sets a skin for the over state of the closebox
	- pressed: sets a skin for the pressed state of the closebox
	
	\htmlonly
	<ul>
	<span class="S1">&lt;minimizebox</span><span class="S8"> </span><span class="S3">relate_to</span><span class="S8">=</span><span class="S6">"top"</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"640,4,-1,-1"</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"minimize.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"minimize_over.png"</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	This sets a minimize box for the frame See closebox above. 
	\htmlonly
	<ul>
	<span class="S1">&lt;maximizebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"0,0,0,0"</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	This sets a maximize box for the frame See closebox above. 
	\htmlonly
	<ul>
	<span class="S1">&lt;titlebar</span><span class="S8"> </span><span class="S3">height</span><span class="S8">=</span><span class="S6">"25"</span><span class="S11">/&gt;</span><
	</ul>
	\endhtmlonly
	Specifies the height of the titlebar. The title bar area will fit the one of the "top" skin. The default height is 10.
	\htmlonly
	<ul>
	<span class="S1">&lt;titleposition</span><span class="S8"> </span><span class="S3">placement</span><span class="S8">=</span><span class="S6">"middle"</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	"titleposition" specifies where the title is placed inside the titlebar. "placement" can either be "left, "middle" or "right". The title will be placed accordingly.
	\htmlonly
	<ul>
	<span class="S1">&lt;titlefont</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">"Comic Sans MS"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"12"</span><span class="S8"> </span><span class="S3">weight</span><span class="S8">=</span><span class="S6">"normal"</span><span class="S8"> </span><span class="S3">underline</span><span class="S8">=</span><span class="S6">"false"</span><span class="S8"> </span><span class="S3">italic</span><span class="S8">=</span><span class="S6">"false"</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	"titlefont"  specifies a font to use in the titlebar. The font has to be installed on the system. Later on wxSkin may add a feature to load these at runtime.
	- name: The facename of the font. This is not a filename.
	- size: The pointsize of the font.
	- weight: Can either be "normal", "light" or "bold".
	- underline: Either "true" or "false". Default is false.
	- italic: Either "true" or "false". Default is false.
	
	There can be multiple wxSkinFrame definitions on one skin file.
	
	\section xml_controls Definitions for Controls
	wxSkin can control the skinning of all controls it defines through XML.
	
	Controls are grouped together for readability. There can be multiple occurances of groups in a skin file. wxSkin defines one special definition, called "default". "default" is defined like any other
	control with a label, but it may occur only *once* for all control groups.
	\subsection xml_contr_btn wxSkinButton
	The "Buttons" group groups definitions for wxSkinButtons.
	\htmlonly
	<ul>
<span><span class="S1">&lt;Buttons&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;default</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"def_btn.png"</span><span class="S8"> </span><span class="S3">pressed</span><span class="S8">=</span><span class="S6">"def_btn_pressed.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8"> =</span><span class="S6">"def_button_over.png"</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">"def_btn_dis.png"</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">"true"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;button_open</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"open.png"</span><span class="S8"> </span><span class="S3">pressed</span><span class="S8">=</span><span class="S6">"open_pressed.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8"> =</span><span class="S6">"open_over.png"</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">"open_dis.png"</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">"70,92"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"-1,-1"</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">"true"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;/Buttons&gt;</span><span class="S0"> </span></span>
	</ul>
	\endhtmlonly
	Each button is definied by a "label" and some properties (with the exception of the "default" label).
	\htmlonly
	<ul>
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;button_open</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"open.png"</span><span class="S8"> </span><span class="S3">pressed</span><span class="S8">=</span><span class="S6">"open_pressed.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8"> =</span><span class="S6">"open_over.png"</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">"open_dis.png"</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">"70,92"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"-1,-1"</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">"true"</span><span class="S11">/&gt;</span>
	</ul>
	\endhtmlonly
	- button_open: the label for the button which is used when assigning "real" controls to skin definition.
	- normal: a skin for "normal" state
	- over: a skin for "over" state
	- pressed: a skin for "pressed" state
	- disabled: a skin for "disabled" state
	- pos: the position of the button in the form "x,y". When adding a button to a sizer this is merely a hint for initial creation.
	- size: the size of the button in the form "width,height". When adding a button to a sizer this is merely a hint for initial creation.
	- shown: controls wether a button is initialy shown. May be "true" (default) or "false". 
	
	The "default" label is a bit special. It doesn't have a pos or size property. "default"s are used for all controls of a grouped kind that don't have any other skins assigned. Later on wxSkin may
	implement a feature that will set these skins automatically to all controls that don't have special skins assigned and are not even "registered" through wxSkinEngine. Currently this only works for
	assigned controls missing a special skin assignment.
	\subsection xml_contr_chk wxSkinCheckBox
	The "Checkboxes" group groups definitions for wxSkinCheckboxes.
	\htmlonly
	<ul>
	<span><span class="S1">&lt;Checkboxes&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;mute</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"mute.png"</span><span class="S8"> </span><span class="S3">checked</span><span class="S8">=</span><span class="S6">"mute_check.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"mute_over.png"</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">"mute_dis.png"</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">"41,249"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"-1,-1"</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">"true"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;/Checkboxes&gt;</span></span>
	</ul>
	\endhtmlonly
	- mute: the label for the button which is used when assigning "real" controls to skin definition.
	- normal: a skin for "normal" state
	- checked: a skin for "checked" state
	- over: a skin for "over" state
	- disabled: a skin for "disabled" state
	- pos: the position of the button in the form "x,y". When adding a button to a sizer this is merely a hint for initial creation.
	- size: the size of the button in the form "width,height". When adding a button to a sizer this is merely a hint for initial creation.
	- shown: controls wether a button is initialy shown. May be "true" (default) or "false". 
	
	For a "default" skin definition, see \ref xml_contr_btn above.
	\subsection xml_contr_rd wxSkinRadiobutton
	The "Radiobuttons" group groups definitions for wxSkinRadiobuttons.
	\htmlonly
	<ul>
	<span><span class="S1">&lt;Radiobuttons&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;radio_select</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"radio.png"</span><span class="S8"> </span><span class="S3">checked</span><span class="S8">=</span><span class="S6">"radio_check.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"radio_over.png"</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">"radio_dis.png"</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">"41,249"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"-1,-1"</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">"true"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;/Radiobuttons&gt;</span></span>
	</ul>
	\endhtmlonly
	In general this follows the same guidelines as \ref xml_contr_chk above. wxSkinRadiobuttons can form groups like normal radio buttons. When selecting one button, the others will be deselected.
	Note that this does not work in a combination of "standard" and skinned radio buttons.
	\subsection xml_contr_g wxSkinGauge
	The "Gauges" group groups definitions for wxSkinGauges.
	\htmlonly
	<ul>
	<span><span class="S1">&lt;Gauges&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;progress</span><span class="S8"> </span><span class="S3">foreground</span><span class="S8">=</span><span class="S6">"fore.png"</span><span class="S8"> </span><span class="S3">background</span><span class="S8">=</span><span class="S6">"back.png"</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">"dis.png"</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">"41,249"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"-1,-1"</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">"true"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;/Gauges&gt;</span></span>
	</ul>
	\endhtmlonly
	- foreground: a skin for the progress bar (the thing that moves :-))
	- background: a skin for the background (the "empty" control)
	- disabled: a skin for "disabled" state
	- pos: the position of the button in the form "x,y". When adding a button to a sizer this is merely a hint for initial creation.
	- size: the size of the button in the form "width,height". When adding a button to a sizer this is merely a hint for initial creation.
	- shown: controls wether a button is initialy shown. May be "true" (default) or "false". 
	\subsection xml_contr_sld wxSkinSlider
	The "Sliders" group groups definitions for wxSkinSliders.
	\htmlonly
	<ul>
	<span><span class="S1">&lt;Sliders&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;progressslide</span><span class="S8"> </span><span class="S3">slider</span><span class="S8">=</span><span class="S6">"position.png"</span><span class="S8"> </span><span class="S3">background</span><span class="S8">=</span><span class="S6">"position_back.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"position_over.png"</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">"position_dis.png"</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">"177,297"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"-1,-1"</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">"true"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;/Sliders&gt;</span></span>
	</ul>
	\endhtmlonly
	- slider: a skin for the "thumb" a user can move
	- background: a skin for the background the slider is moved on
	- over: a skin for the thumb in "over" state
	- disbaled: a skin for "disabled" state
	- pos: the position of the button in the form "x,y". When adding a button to a sizer this is merely a hint for initial creation.
	- size: the size of the button in the form "width,height". When adding a button to a sizer this is merely a hint for initial creation.
	- shown: controls wether a button is initialy shown. May be "true" (default) or "false". 
	
	Note: This class is still under heavy development. This definitions may change, although it is unlikely.
	\subsection xml_contr_p wxSkinPanel
	The "Panels" group groups definitions for wxSkinPanels.
	\htmlonly
	<ul>
	<span><span class="S1">&lt;Panels&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;panel</span><span class="S8"> </span><span class="S3">background</span><span class="S8">=</span><span class="S6">"panel_back.png"</span><span class="S8"> &nbsp;</span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">"panel_dis.png"</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">"177,297"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"-1,-1"</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">"true"</span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;/Panels&gt;</span></span>
	</ul>
	\endhtmlonly
	- background: a skin sued for the (shaped) background
	- disabled: a skin for "disabled" state
	- pos: the position of the button in the form "x,y". When adding a button to a sizer this is merely a hint for initial creation.
	- size: the size of the button in the form "width,height". When adding a button to a sizer this is merely a hint for initial creation.
	- shown: controls wether a button is initialy shown. May be "true" (default) or "false". 
	
	Note: This is only a utility class. It may be decided that it has no real purpose and be removed. Currently I can't think of a real usecase, but I decided to leave it for convenience.
	\subsection xml_contr_stat wxSkinStaticText
	This control doesn't have and need a special group. It can be controled by using \ref xml_widgets.
	
	\section xml_widgets Definitions for custom widgets
	The "Widgets" group is a special group used to control any other widget that is assigned but not controled in another way. It can be used to control "standard" widgets as well.
	\htmlonly
	<ul>
	<span><span class="S1">&lt;Widgets&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;control</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">"444,287"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"-1,-1"</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">"true"</span><span class="S8"> </span><span class="S11">/&gt;</span><br />
<span class="S1">&lt;/Widgets&gt;</span></span>
	</ul>
	\endhtmlonly
	- pos: the position of the button in the form "x,y". When adding a button to a sizer this is merely a hint for initial creation.
	- size: the size of the button in the form "width,height". When adding a button to a sizer this is merely a hint for initial creation.
	- shown: controls wether a button is initialy shown. May be "true" (default) or "false". 
	
	Note: The "Widgets" group can't have a "default" definition.
	\section xml_all A complete wxSkin XML template
	Just for convenience, here's an empty wxSkin file template. Adjust it to suit your needs.
	\htmlonly
	<ul>
	<span><span class="S12">&lt;?</span><span class="S1">xml</span><span class="S8"> </span><span class="S3">version</span><span class="S8">=</span><span class="S6">"1.0"</span><span class="S8"> </span><span class="S3">encoding</span><span class="S8">=</span><span class="S6">"ISO-8859-1"</span><span class="S13">?&gt;</span><br />
<br />
<span class="S1">&lt;Skin</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">author</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">comment</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">screenshot</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">application</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">customInfo</span><span class="S8">=</span><span class="S6">""</span><span class="S1">&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;Transparency</span><span class="S8"> </span><span class="S3">red</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">green</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">blue</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;Buttons&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;default</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pressed</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8"> =</span><span class="S6">""</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;label_button</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pressed</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8"> =</span><span class="S6">""</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;/Buttons&gt;</span><span class="S0"> </span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;Checkboxes&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;default</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">checked</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;label_checkbox</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">checked</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;/Checkboxes&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;Radiobuttons&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;default</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">checked</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;label_radio</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">checked</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;/Radiobuttons&gt;</span><br />
<br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;Gauges&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;default</span><span class="S8"> </span><span class="S3">foreground</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">background</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;label_gauge</span><span class="S8"> </span><span class="S3">foreground</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">background</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;/Gauges&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;Sliders&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;default</span><span class="S8"> </span><span class="S3">slider</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">background</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;label_slider</span><span class="S8"> </span><span class="S3">slider</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">background</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;/Sliders&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;Panels&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;default</span><span class="S8"> </span><span class="S3">background</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> &nbsp;</span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;label_panel</span><span class="S8"> </span><span class="S3">background</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> &nbsp;</span><span class="S3">disabled</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;/Panels&gt;</span><br />
<br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;Widgets&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;label_control</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">shown</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;/Widgets&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;SimpleFrame</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">movable</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> &nbsp;</span><span class="S3">guibuilder</span><span class="S8">=</span><span class="S6">""</span><span class="S1">&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;client_area</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;closebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;minimizebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;maximizebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titlebar</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titleposition</span><span class="S8"> </span><span class="S3">placement</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titlefont</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">weight</span><span class="S8">=</span><span class="S6">"l"</span><span class="S8"> </span><span class="S3">underline</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">italic</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;/SimpleFrame&gt;</span><span class="S0"> </span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;Frame</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">movable</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">guibuilder</span><span class="S8">=</span><span class="S6">""</span><span class="S1">&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;top</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;top_left</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;top_right</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;bottom</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;bottom_left</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;bottom_right</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;left</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"l"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;right</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;body</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;closebox</span><span class="S8"> </span><span class="S3">relate_to</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;minimizebox</span><span class="S8"> </span><span class="S3">relate_to</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;maximizebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titlebar</span><span class="S8"> </span><span class="S3">height</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titleposition</span><span class="S8"> </span><span class="S3">placement</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;titlefont</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">weight</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">underline</span><span class="S8">=</span><span class="S6">""</span><span class="S8"> </span><span class="S3">italic</span><span class="S8">=</span><span class="S6">""</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp; &nbsp; &nbsp; &nbsp; </span><span class="S1">&lt;/Frame&gt;</span><span class="S0"> </span><br />
<span class="S1">&lt;/Skin&gt;</span><br />
<span class="S0"></span></span>
	</ul>
	\endhtmlonly
*/