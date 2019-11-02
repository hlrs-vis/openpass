#!/bin/bash

#:: /*********************************************************************
#:: * This Example Content is intended to demonstrate usage of
#:: * Eclipse technology. It is provided to you under the terms
#:: * and conditions of the Eclipse Distribution License v1.0
#:: * which is available at http://www.eclipse.org/org/documents/edl-v10.php
#:: **********************************************************************/

rm -rf html

[ -d "../../Source" ] && SourceDir='../../Source'
[ ! -d "../../Source" ] && SourceDir='../../OpenPass_Source_Code'

export SourceDir

OSDocumentationDir='../'

export OSDocumentationDir

doxygen Function.doxy -d markdown

mv html/openPASS_Function_Docu.chm ../openPASS_Function_Docu.chm
rm -rf html
