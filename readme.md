Work in progress 
* it's intended to replace the backend used in vrtracker.
* uses TBB to scrape openvr and compress textures in parallel.
* meant to support the idea of a bridge (or a pipe say) so that openvr calls can be intercepted and values from a capture can be returned instead of the live openvr instance.
* needs yet another refactor to derive the schema and poll openvr from a json file instead of a baked in schema

To build
* open submodules/oneTBB/build/vs2012 using visual studio 2017
* build the Debug-MT target for 64 bits
* then you can build the rangesplay.sln
