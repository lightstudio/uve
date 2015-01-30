WP AppResLib DLL Generator V1.1 - Written by Peter Tihanyi
==========================================================

Description:
------------
When you're creating an application for Windows Phone, you may need to
localize it to multiple languages. Inside the application it's doable, the
method is up to you, but there is a way to localize the Application Title
and the title on the Tile you pin to the home screen. This requires resource
DLL files for each supported language, which is a bit of a pain to create.

This tool simplifies the process. Just create your project config XML file
based on the provided Sample.xml and generate the DLL files using my
template file AppResLib.bin which has no debug information, only two 80
characters long Unicode placeholders to replace.

Refer to the file Locale_ID_List.txt to figure out the Locale ID of your
chosen language(s), use the Hexadecimal number (like 0409 - US English)

You only need to add the languages you actually support, plus the Neutral one.


Usage:
------
WPAppResLib.exe <project config xml file>

The DLL files will be placed in a directory named after your project, in the
same directory where the tool is located.

Once you have the files generated, copy them to the root of your project and
add them to the project files. Make sure you set the Build Action to Content
on each of them, in their Properties window.

Then you have to open your WMAppManifest.xml file and edit it a little, but
actually it's easier to do it in the project properties window's Application
section.

Application Title: "@AppResLib.dll,-100"  [<App Title="@AppResLib.dll,-100" ]

Tile Title: "@AppResLib.dll,-200" [<TemplateType5><Title>@AppResLib.dll,-200 ]

Also, make sure you open the project properties window (again) and click on
the "Assembly Information..." button, then in the dialog you select a
Neutral Language on the bottom. Usually your application's neutral language
will be English, so select "English". Not "English (United States)" or other
variations... If you don't follow this, your app will be rejected on upload to
the Marketplace with an error message.


Let me know if you found it useful!


Sincerely,

Peter -- http://engine-designs.com


PS: Article about usage and download:

http://engine-designs.com/wp7-appreslib-dll-generator.html
