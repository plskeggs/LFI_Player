The software supports 3 different kinds of laser content files, ILDA (.ild), LFIP abstracts (.abs), and text for displaying scrolling text (.txt).  You can open these directly, but typically they’re opened and manipulated by a LFIP script file (.cts).  You can then trigger the scripts by pressing buttons on a Live Control panel which is configured in a live control configuration file (.clv).  Note that the Live Control panel has 12 “pages”, with each page triggering up to 48 different script queues.
 
In the folder LFIP_Demo you will find Demo.clv, along with folders for the content files and scripts.  Note that the .clv and .cts files are just text files that you can open and review if you’re interested.  Note also that if you place the files in a different location, you can specify the Content Base Directory from the Live Control tab in Options -> Preferences.
 
Since the abstracts and associated abstract generator is the best part (or at least one of the best parts) of LFI Player, in order to open the abstract generator console, make sure you have an existing .abs file opened (either directly, or opened via a .cts script), and then click on the “Track Focus” button (main control panel, upper left part of the window) for the track associated with that abstract.  The control panel should appear.
 
Another fun thing (IMO) is to manipulate the perspective of what’s being displayed by taking the mouse (or presumably finger if you have a touch screen which I don’t) and “spinning” the image (or images) by dragging.  Note that this feature has something that I call inertia, so you can spin the image, let it go with the mouse, and it will continue to spin and then slow to a stop depending on how much inertia you have set in the preferences.  A lot of inertia, and it will continue to spin for a long time.  Less inertia, and it will come to a stop more quickly.  Once you get the hang of this feature, you can get some really cool effects, particularly with 3D abstracts and a wide “Camera Angle”.