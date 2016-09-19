
WINDOWS_7_64 BUILD BOOST
 1. After extract boost
 2. 
    Go to the directory tools\build\v2\.
    Run bootstrap.bat
	cd boost root directory

 3. Build Visual Studio 2010
    3.1: .\tools\build\v2\b2 --build-dir=build-directory toolset=msvc-10.0 architecture=x86 address-model=64 --build-type=complete stage
	3.2: .\tools\build\v2\b2 --build-dir=build-directory toolset=msvc-10.0 architecture=x86 address-model=64 --build-type=complete install --prefix=PREFIX
         where PREFIX is the directory where you want Boost.Build to be installed
    3.3: <optional> PREFIX\bin to your PATH environment variable.
  
 4. Build gcc mingw
    4.1: .\tools\build\v2\b2 --build-dir=build-directory toolset=gcc  architecture=x86 --build-type=complete stage
	     WHERE: build-dir can be "c:\opencv\..."