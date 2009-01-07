= Mocap File Importer 0.85 beta =

Copyright 2005-2009 Shun Cox
http://code.google.com/p/mocapfileimporter


== Description ==

This plug-in is used for importing BVH/HTR file (support Autodesk Maya only). More mocap file formats will be supported in future.
Neither Mac OS X nor Windows/Linux 64bit has been supported yet.


== License ==

This code is released under the GPL.  Within directories you will find the code as well as the full GPL license (license.txt). If you use the launcher or code in your own product, please give proper and prominent attribution.
Mocap File Importer is free for non-commercial use. Commercial use is forbidden without permission.


== Installation ==

  # Copy imocaputilz.mll or imocaputilz.so to your maya plug-ins directory. (For example: "C:\Program Files\Alias\Maya8.5\bin\plug-ins")
  # Copy imocapImportOptions.mel to your maya scripts directory. (For example: "C:\Documents and Settings\user\My Documents\maya\8.5\scripts")
  # Launch maya and load this plug-in.


== Usage ==

Main menu 'File -> Open / Import'
Attention: Before applying mocap data to a skeleton, you should select the root joint of it first.


== History ==

  * Mocap File Importer 0.85 beta
    * Added support for Maya 8.0 and 8.5
    * Fixed filetype issue in open/import dialog
    * Fixed some minor bugs

  * Mocap File Importer 0.8 beta
    * Added applying of data to an existing skeleton
    * Added Linux version

  * Mocap File Importer 0.7 beta
    * Support for HTR (Version 1) file
    * Fixed some minor bugs

  * Mocap File Importer 0.61
    * Fixed freezing when catching spaced joint names in a BVH file
    * Fixed freezing for some text files whose lines was broked by '0d'
    * Thanks to John LeBaron, who helped me in finding out these bugs

  * Mocap File Importer 0.6 beta
    * Support for BVH file
    * Support for Maya namespace
    * Enhanced performance by using of native Maya API
    * More...


== Support ==

For any help or comments please e-mail to shuncox(AT)gmail.com

