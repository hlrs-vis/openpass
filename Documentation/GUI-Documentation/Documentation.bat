set HHC="C:\Program Files (x86)\HTML Help Workshop\hhc.exe"
REM line3 needs to be commented out to modify automatic insertion from doxygen
REM doxygen Documentation.doxyfile
copy /Y Documentation.index.hhc html\index.hhc
%HHC% html\index.hhp
move /Y html\index.chm Documentation.chm
REM rmdir /S/Q html