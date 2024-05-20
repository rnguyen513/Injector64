#  Simple LoadLibrary DLL injector for x64
CLI-based and works based off of the process ID. Call it using <strong>.\LibLoader.exe [pid]</strong> (use task manager or other tool to find pid)

###  How it works


First, figure out what the target process is based on the provided process ID (PID). Then, open Kernel32.dll and get the LoadLibraryW function (W for wide, as opposed to ANSI)
![Screenshot 2024-05-19 233343](https://github.com/rnguyen513/Injector64/assets/77138030/ac9202ef-b58b-45c3-9b84-57134536cdfc)

Allocate buffer region somewhere random in the process memory space (can change the second parameter from NULL to whatever address, but by default the system finds a random spot for you). After instantitating that buffer, we can write the path (wPath) to our payload DLL to it.
![Screenshot 2024-05-19 233929](https://github.com/rnguyen513/Injector64/assets/77138030/536d7ba2-9a9f-494f-b161-0fab35176256)

Then, start up the main injection thread. Here, we pass in p_LoadLibrary as the instruction to be executed by the thread, and the following parameter, buffer, is the argument passed to the function which the thread executes (which is LoadLibraryW that we just specified!). Nice, LoadLibraryW now takes the allocated buffer region as input, and injects it to the process.
![Screenshot 2024-05-19 233616](https://github.com/rnguyen513/Injector64/assets/77138030/77578221-11f2-48cd-a170-c82c67be111a)

At this point, our payload DLL is fully injected into the process and we'll wait for it to terminate. When it does, we'll clean up by closing our handles to the process, thread, and also clear up the buffer region that we allocated prior.
![Screenshot 2024-05-19 233639](https://github.com/rnguyen513/Injector64/assets/77138030/05b617de-acc5-4db8-8c2c-1aff8d8873f8)


###  Pros
+Any non-protected process is injectable (although perhaps not safe) as long as you know the PID

+Lightweight and only allocates minimum memory needed

###  Cons
-Does not check if DLL to be injected is valid, LoadLibrary will always go try to load the buffer even if its random memory (hopefully them MS devs put checks!!)

-Path of DLL to inject is hardcoded currently. Initially had it configured so a path could be specified when calling the tool, but the conversions just didn't work

-Its LoadLibrary lol, simplest and most easily detected injection
