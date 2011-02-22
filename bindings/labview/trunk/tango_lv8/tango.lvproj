<?xml version='1.0'?>
<Project Type="Project" LVVersion="8508002">
   <Item Name="My Computer" Type="My Computer">
      <Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
      <Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
      <Property Name="server.tcp.enabled" Type="Bool">false</Property>
      <Property Name="server.tcp.port" Type="Int">0</Property>
      <Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
      <Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
      <Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
      <Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
      <Property Name="specify.custom.address" Type="Bool">false</Property>
      <Item Name="examples" Type="Folder">
         <Item Name="sub-vis" Type="Folder">
            <Item Name="RandomBoolArray.vi" Type="VI" URL="../examples_lv8/sub-vis/RandomBoolArray.vi"/>
            <Item Name="RandomValue.vi" Type="VI" URL="../examples_lv8/sub-vis/RandomValue.vi"/>
            <Item Name="RandomArray.vi" Type="VI" URL="../examples_lv8/sub-vis/RandomArray.vi"/>
         </Item>
         <Item Name="perfs" Type="Folder">
            <Item Name="ReadTangoTestDoubleAttributes.vi" Type="VI" URL="../examples_lv8/perfs/ReadTangoTestDoubleAttributes.vi"/>
            <Item Name="ReadTangoTestLongAttributes.vi" Type="VI" URL="../examples_lv8/perfs/ReadTangoTestLongAttributes.vi"/>
            <Item Name="ReadTangoTestShortAttributes.vi" Type="VI" URL="../examples_lv8/perfs/ReadTangoTestShortAttributes.vi"/>
         </Item>
         <Item Name="CommandInOutTest.vi" Type="VI" URL="../examples_lv8/CommandInOutTest.vi"/>
         <Item Name="ReadAttributesTest.vi" Type="VI" URL="../examples_lv8/ReadAttributesTest.vi"/>
         <Item Name="ReadAttributeTest.vi" Type="VI" URL="../examples_lv8/ReadAttributeTest.vi"/>
         <Item Name="ReadStringSpectrum.vi" Type="VI" URL="../examples_lv8/ReadStringSpectrum.vi"/>
         <Item Name="StateScalarStateColorStateName.vi" Type="VI" URL="../examples_lv8/StateScalarStateColorStateName.vi"/>
         <Item Name="WriteAttributesTest.vi" Type="VI" URL="../examples_lv8/WriteAttributesTest.vi"/>
         <Item Name="WriteAttributeTest.vi" Type="VI" URL="../examples_lv8/WriteAttributeTest.vi"/>
         <Item Name="WrittingSpectrumAndImageAttributes.vi" Type="VI" URL="../examples_lv8/WrittingSpectrumAndImageAttributes.vi"/>
         <Item Name="TangoGroup.vi" Type="VI" URL="../examples_lv8/TangoGroup.vi"/>
         <Item Name="ExtractArchivedSpectrum.vi" Type="VI" URL="../examples_lv8/ExtractArchivedSpectrum.vi"/>
         <Item Name="ExtractArchivedData.vi" Type="VI" URL="../examples_lv8/ExtractArchivedData.vi"/>
         <Item Name="TangoEventsTest.vi" Type="VI" URL="../examples_lv8/TangoEventsTest.vi"/>
      </Item>
      <Item Name="tango_binding.dll" Type="Document" URL="tango_binding.dll"/>
      <Item Name="tango_vi.llb" Type="Document" URL="tango_vi.llb"/>
      <Item Name="Dependencies" Type="Dependencies">
         <Item Name="vi.lib" Type="Folder">
            <Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
            <Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
            <Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
            <Item Name="General Error Handler CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler CORE.vi"/>
            <Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
            <Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
            <Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
            <Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
            <Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
            <Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
            <Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
            <Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
            <Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
            <Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
            <Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
            <Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
            <Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
            <Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
            <Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
            <Item Name="Not Found Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Not Found Dialog.vi"/>
            <Item Name="Three Button Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog.vi"/>
            <Item Name="Three Button Dialog CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog CORE.vi"/>
            <Item Name="Longest Line Length in Pixels.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Longest Line Length in Pixels.vi"/>
            <Item Name="Convert property node font to graphics font.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Convert property node font to graphics font.vi"/>
            <Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
            <Item Name="BuildHelpPath.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/BuildHelpPath.vi"/>
            <Item Name="GetHelpDir.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetHelpDir.vi"/>
            <Item Name="Waveform Array To Dynamic.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/transition.llb/Waveform Array To Dynamic.vi"/>
            <Item Name="subBuildXYGraph.vi" Type="VI" URL="/&lt;vilib&gt;/express/express controls/BuildXYGraphBlock.llb/subBuildXYGraph.vi"/>
            <Item Name="Dynamic To Waveform Array.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/transition.llb/Dynamic To Waveform Array.vi"/>
            <Item Name="ex_CorrectErrorChain.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_CorrectErrorChain.vi"/>
         </Item>
         <Item Name="tango.lvlib" Type="Library" URL="tango_vi.llb/tango.lvlib"/>
         <Item Name="tango_binding.dll" Type="Document" URL="tango_binding.dll"/>
      </Item>
      <Item Name="Build Specifications" Type="Build">
         <Item Name="tango-lv8-dist" Type="Source Distribution">
            <Property Name="Bld_buildSpecName" Type="Str">tango-lv8-dist</Property>
            <Property Name="Bld_excludedDirectoryCount" Type="Int">3</Property>
            <Property Name="Bld_excludedDirectory[0]" Type="Path">vi.lib</Property>
            <Property Name="Bld_excludedDirectory[0].pathType" Type="Str">relativeToAppDir</Property>
            <Property Name="Bld_excludedDirectory[1]" Type="Path">instr.lib</Property>
            <Property Name="Bld_excludedDirectory[1].pathType" Type="Str">relativeToAppDir</Property>
            <Property Name="Bld_excludedDirectory[2]" Type="Path">user.lib</Property>
            <Property Name="Bld_excludedDirectory[2].pathType" Type="Str">relativeToAppDir</Property>
            <Property Name="DestinationCount" Type="Int">2</Property>
            <Property Name="Destination[0].destName" Type="Str">Répertoire de destination</Property>
            <Property Name="Destination[0].path" Type="Path">../builds/tango_vi.llb</Property>
            <Property Name="Destination[0].type" Type="Str">LLB</Property>
            <Property Name="Destination[1].destName" Type="Str">Répertoire de support</Property>
            <Property Name="Destination[1].path" Type="Path">../builds/tango-lv8-dist/data</Property>
            <Property Name="SourceCount" Type="Int">5</Property>
            <Property Name="SourceItem[100].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[100].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[100].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[101].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[101].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[101].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[102].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[102].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[102].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[103].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[103].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[103].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[105].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[105].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[105].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[106].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[106].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[106].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[107].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[107].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[107].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[108].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[108].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[108].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[109].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[109].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[109].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[110].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[110].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[110].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[111].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[111].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[111].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[112].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[112].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[112].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[113].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[113].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[113].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[115].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[115].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[115].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[118].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[118].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[118].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[119].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[119].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[119].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[120].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[120].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[120].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[121].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[121].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[121].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[122].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[122].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[122].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[123].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[123].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[123].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[124].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[124].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[124].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[125].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[125].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[125].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[126].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[126].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[126].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[128].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[128].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[128].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[129].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[129].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[129].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[130].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[130].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[130].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[131].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[131].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[131].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[132].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[132].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[132].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[133].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[133].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[133].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[134].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[134].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[134].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[135].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[135].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[135].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[136].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[136].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[136].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[138].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[138].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[138].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[141].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[141].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[141].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[142].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[142].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[142].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[143].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[143].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[143].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[144].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[144].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[144].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[145].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[145].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[145].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[146].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[146].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[146].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[147].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[147].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[147].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[148].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[148].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[148].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[149].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[149].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[149].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[151].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[151].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[151].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[152].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[152].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[152].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[153].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[153].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[153].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[154].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[154].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[154].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[155].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[155].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[155].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[156].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[156].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[156].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[157].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[157].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[157].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[158].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[158].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[158].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[159].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[159].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[159].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[171].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[171].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[171].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[172].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[172].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[172].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[173].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[173].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[173].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[174].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[174].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[174].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[175].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[175].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[175].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[176].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[176].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[176].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[177].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[177].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[177].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[178].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[178].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[178].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[179].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[179].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[179].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[180].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[180].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[180].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[181].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[181].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[181].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[182].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[182].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[182].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[183].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[183].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[183].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[184].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[184].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[184].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[185].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[185].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[185].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[186].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[186].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[186].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[92].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[92].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[92].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[95].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[95].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[95].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[96].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[96].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[96].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[97].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[97].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[97].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[98].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[98].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[98].TopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[99].FolderTopLevelVI" Type="Str">Never</Property>
            <Property Name="SourceItem[99].ItemID" Type="Ref"></Property>
            <Property Name="SourceItem[99].TopLevelVI" Type="Str">Never</Property>
            <Property Name="Source[0].Container.applyDestination" Type="Bool">true</Property>
            <Property Name="Source[0].Container.applyInclusion" Type="Bool">true</Property>
            <Property Name="Source[0].Container.applyPassword" Type="Bool">true</Property>
            <Property Name="Source[0].Container.applyProperties" Type="Bool">true</Property>
            <Property Name="Source[0].itemID" Type="Str">{A1E37A9F-23BF-11DF-A973-001D09650C2E}</Property>
            <Property Name="Source[0].type" Type="Str">Container</Property>
            <Property Name="Source[1].Container.applyInclusion" Type="Bool">true</Property>
            <Property Name="Source[1].destinationIndex" Type="Int">0</Property>
            <Property Name="Source[1].itemID" Type="Ref"></Property>
            <Property Name="Source[1].sourceInclusion" Type="Str">Include</Property>
            <Property Name="Source[1].type" Type="Str">Container</Property>
            <Property Name="Source[2].destinationIndex" Type="Int">0</Property>
            <Property Name="Source[2].itemID" Type="Ref"></Property>
            <Property Name="Source[2].Library.allowMissingMembers" Type="Bool">true</Property>
            <Property Name="Source[2].sourceInclusion" Type="Str">Include</Property>
            <Property Name="Source[2].type" Type="Str">Library</Property>
            <Property Name="Source[3].Container.applyInclusion" Type="Bool">true</Property>
            <Property Name="Source[3].destinationIndex" Type="Int">0</Property>
            <Property Name="Source[3].itemID" Type="Ref">/My Computer/examples</Property>
            <Property Name="Source[3].sourceInclusion" Type="Str">Include</Property>
            <Property Name="Source[3].type" Type="Str">Container</Property>
            <Property Name="Source[4].destinationIndex" Type="Int">0</Property>
            <Property Name="Source[4].itemID" Type="Ref"></Property>
            <Property Name="Source[4].sourceInclusion" Type="Str">Include</Property>
            <Property Name="VersionInfoCompanyName" Type="Str"></Property>
            <Property Name="VersionInfoFileDescription" Type="Str"></Property>
            <Property Name="VersionInfoFileType" Type="Int">0</Property>
            <Property Name="VersionInfoFileVersionBuild" Type="Int">0</Property>
            <Property Name="VersionInfoFileVersionMajor" Type="Int">0</Property>
            <Property Name="VersionInfoFileVersionMinor" Type="Int">0</Property>
            <Property Name="VersionInfoFileVersionPatch" Type="Int">0</Property>
            <Property Name="VersionInfoInternalName" Type="Str"></Property>
            <Property Name="VersionInfoLegalCopyright" Type="Str"></Property>
            <Property Name="VersionInfoProductName" Type="Str"></Property>
         </Item>
      </Item>
   </Item>
</Project>
