del dwn.obj dwn.exe
cl /O1 /GS- /Gs999999 dwn.c /c
link /out:dwn.exe dwn.obj kernel32.lib wininet.lib /ENTRY:MyStart /SUBSYSTEM:CONSOLE /NODEFAULTLIB /OPT:REF /OPT:ICF /FILEALIGN:512 /MERGE:.rdata=.text /MERGE:.pdata=.text /RELEASE /DYNAMICBASE:NO /NXCOMPAT:NO