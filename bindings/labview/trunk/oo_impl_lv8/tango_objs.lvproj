<?xml version='1.0'?>
<Project Type="Project" LVVersion="8208000">
   <Item Name="Poste de travail" Type="My Computer">
      <Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
      <Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
      <Property Name="server.tcp.enabled" Type="Bool">false</Property>
      <Property Name="server.tcp.port" Type="Int">0</Property>
      <Property Name="server.tcp.serviceName" Type="Str">Poste de travail/VI Serveur</Property>
      <Property Name="server.tcp.serviceName.default" Type="Str">Poste de travail/VI Serveur</Property>
      <Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
      <Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
      <Property Name="specify.custom.address" Type="Bool">false</Property>
      <Item Name="TangoObjs.lvlib" Type="Library" URL="TangoObjs.lvlib">
         <Item Name="Base" Type="Folder">
            <Item Name="Command" Type="Folder">
               <Item Name="Extractors" Type="Folder">
                  <Item Name="fromDevBoolean.vi" Type="VI" URL="Base/Base.llb/fromDevBoolean.vi"/>
                  <Item Name="fromDevDouble.vi" Type="VI" URL="Base/Base.llb/fromDevDouble.vi"/>
                  <Item Name="fromDevFloat.vi" Type="VI" URL="Base/Base.llb/fromDevFloat.vi"/>
                  <Item Name="fromDevLong.vi" Type="VI" URL="Base/Base.llb/fromDevLong.vi"/>
                  <Item Name="fromDevShort.vi" Type="VI" URL="Base/Base.llb/fromDevShort.vi"/>
                  <Item Name="fromDevString.vi" Type="VI" URL="Base/Base.llb/fromDevString.vi"/>
                  <Item Name="fromDevULong.vi" Type="VI" URL="Base/Base.llb/fromDevULong.vi"/>
                  <Item Name="fromDevUShort.vi" Type="VI" URL="Base/Base.llb/fromDevUShort.vi"/>
                  <Item Name="fromDevVarCharArray.vi" Type="VI" URL="Base/Base.llb/fromDevVarCharArray.vi"/>
                  <Item Name="fromDevVarDoubleArray.vi" Type="VI" URL="Base/Base.llb/fromDevVarDoubleArray.vi"/>
                  <Item Name="fromDevVarDoubleStringArray.vi" Type="VI" URL="Base/Base.llb/fromDevVarDoubleStringArray.vi"/>
                  <Item Name="fromDevVarFloatArray.vi" Type="VI" URL="Base/Base.llb/fromDevVarFloatArray.vi"/>
                  <Item Name="fromDevVarLongArray.vi" Type="VI" URL="Base/Base.llb/fromDevVarLongArray.vi"/>
                  <Item Name="fromDevVarLongStringArray.vi" Type="VI" URL="Base/Base.llb/fromDevVarLongStringArray.vi"/>
                  <Item Name="fromDevVarShortArray.vi" Type="VI" URL="Base/Base.llb/fromDevVarShortArray.vi"/>
                  <Item Name="fromDevVarStringArray.vi" Type="VI" URL="Base/Base.llb/fromDevVarStringArray.vi"/>
                  <Item Name="fromDevVarULongArray.vi" Type="VI" URL="Base/Base.llb/fromDevVarULongArray.vi"/>
                  <Item Name="fromDevVarUShortArray.vi" Type="VI" URL="Base/Base.llb/fromDevVarUShortArray.vi"/>
               </Item>
               <Item Name="Insertors" Type="Folder">
                  <Item Name="toDevBoolean.vi" Type="VI" URL="Base/Base.llb/toDevBoolean.vi"/>
                  <Item Name="toDevDouble.vi" Type="VI" URL="Base/Base.llb/toDevDouble.vi"/>
                  <Item Name="toDevFloat.vi" Type="VI" URL="Base/Base.llb/toDevFloat.vi"/>
                  <Item Name="toDevLong.vi" Type="VI" URL="Base/Base.llb/toDevLong.vi"/>
                  <Item Name="toDevShort.vi" Type="VI" URL="Base/Base.llb/toDevShort.vi"/>
                  <Item Name="toDevString.vi" Type="VI" URL="Base/Base.llb/toDevString.vi"/>
                  <Item Name="toDevULong.vi" Type="VI" URL="Base/Base.llb/toDevULong.vi"/>
                  <Item Name="toDevUShort.vi" Type="VI" URL="Base/Base.llb/toDevUShort.vi"/>
                  <Item Name="toDevVarCharArray.vi" Type="VI" URL="Base/Base.llb/toDevVarCharArray.vi"/>
                  <Item Name="toDevVarDoubleArray.vi" Type="VI" URL="Base/Base.llb/toDevVarDoubleArray.vi"/>
                  <Item Name="toDevVarDoubleStringArray.vi" Type="VI" URL="Base/Base.llb/toDevVarDoubleStringArray.vi"/>
                  <Item Name="toDevVarFloatArray.vi" Type="VI" URL="Base/Base.llb/toDevVarFloatArray.vi"/>
                  <Item Name="toDevVarLongArray.vi" Type="VI" URL="Base/Base.llb/toDevVarLongArray.vi"/>
                  <Item Name="toDevVarLongStringArray.vi" Type="VI" URL="Base/Base.llb/toDevVarLongStringArray.vi"/>
                  <Item Name="toDevVarShortArray.vi" Type="VI" URL="Base/Base.llb/toDevVarShortArray.vi"/>
                  <Item Name="toDevVarStringArray.vi" Type="VI" URL="Base/Base.llb/toDevVarStringArray.vi"/>
                  <Item Name="toDevVarULongArray.vi" Type="VI" URL="Base/Base.llb/toDevVarULongArray.vi"/>
                  <Item Name="toDevVarUShortArray.vi" Type="VI" URL="Base/Base.llb/toDevVarUShortArray.vi"/>
               </Item>
               <Item Name="Argin.vi" Type="VI" URL="Base/Base.llb/Argin.vi"/>
               <Item Name="Argout.vi" Type="VI" URL="Base/Base.llb/Argout.vi"/>
            </Item>
            <Item Name="Group" Type="Folder">
               <Item Name="GroupCreate.vi" Type="VI" URL="Base/Base.llb/GroupCreate.vi"/>
               <Item Name="GroupKill.vi" Type="VI" URL="Base/Base.llb/GroupKill.vi"/>
               <Item Name="GroupReadAttributes.vi" Type="VI" URL="Base/Base.llb/GroupReadAttributes.vi"/>
            </Item>
            <Item Name="Events" Type="Folder">
               <Item Name="EventsSubscribe.vi" Type="VI" URL="Base/Base.llb/EventsSubscribe.vi"/>
               <Item Name="EventsSubscribeToChangeEvt.vi" Type="VI" URL="Base/Base.llb/EventsSubscribeToChangeEvt.vi"/>
               <Item Name="EventsSubscribeToPeriodicEvt.vi" Type="VI" URL="Base/Base.llb/EventsSubscribeToPeriodicEvt.vi"/>
               <Item Name="EventsUnsubscribe.vi" Type="VI" URL="Base/Base.llb/EventsUnsubscribe.vi"/>
            </Item>
            <Item Name="Device" Type="Folder">
               <Item Name="Writers [tmp]" Type="Folder">
                  <Item Name="WriteBooleanImageAttribute.vi" Type="VI" URL="Base/Base.llb/WriteBooleanImageAttribute.vi"/>
                  <Item Name="WriteBooleanScalarAttribute.vi" Type="VI" URL="Base/Base.llb/WriteBooleanScalarAttribute.vi"/>
                  <Item Name="WriteBooleanSpectrumAttribute.vi" Type="VI" URL="Base/Base.llb/WriteBooleanSpectrumAttribute.vi"/>
                  <Item Name="WriteDoubleImageAttribute.vi" Type="VI" URL="Base/Base.llb/WriteDoubleImageAttribute.vi"/>
                  <Item Name="WriteDoubleScalarAttribute.vi" Type="VI" URL="Base/Base.llb/WriteDoubleScalarAttribute.vi"/>
                  <Item Name="WriteDoubleSpectrumAttribute.vi" Type="VI" URL="Base/Base.llb/WriteDoubleSpectrumAttribute.vi"/>
                  <Item Name="WriteFloatImageAttribute.vi" Type="VI" URL="Base/Base.llb/WriteFloatImageAttribute.vi"/>
                  <Item Name="WriteFloatScalarAttribute.vi" Type="VI" URL="Base/Base.llb/WriteFloatScalarAttribute.vi"/>
                  <Item Name="WriteFloatSpectrumAttribute.vi" Type="VI" URL="Base/Base.llb/WriteFloatSpectrumAttribute.vi"/>
                  <Item Name="WriteLongImageAttribute.vi" Type="VI" URL="Base/Base.llb/WriteLongImageAttribute.vi"/>
                  <Item Name="WriteLongScalarAttribute.vi" Type="VI" URL="Base/Base.llb/WriteLongScalarAttribute.vi"/>
                  <Item Name="WriteLongSpectrumAttribute.vi" Type="VI" URL="Base/Base.llb/WriteLongSpectrumAttribute.vi"/>
                  <Item Name="WriteShortImageAttribute.vi" Type="VI" URL="Base/Base.llb/WriteShortImageAttribute.vi"/>
                  <Item Name="WriteShortScalarAttribute.vi" Type="VI" URL="Base/Base.llb/WriteShortScalarAttribute.vi"/>
                  <Item Name="WriteShortSpectrumAttribute.vi" Type="VI" URL="Base/Base.llb/WriteShortSpectrumAttribute.vi"/>
                  <Item Name="WriteStringAttribute.vi" Type="VI" URL="Base/Base.llb/WriteStringAttribute.vi"/>
                  <Item Name="WriteUCharImageAttribute.vi" Type="VI" URL="Base/Base.llb/WriteUCharImageAttribute.vi"/>
                  <Item Name="WriteUCharScalarAttribute.vi" Type="VI" URL="Base/Base.llb/WriteUCharScalarAttribute.vi"/>
                  <Item Name="WriteUCharSpectrumAttribute.vi" Type="VI" URL="Base/Base.llb/WriteUCharSpectrumAttribute.vi"/>
                  <Item Name="WriteUShortImageAttribute.vi" Type="VI" URL="Base/Base.llb/WriteUShortImageAttribute.vi"/>
                  <Item Name="WriteUShortScalarAttribute.vi" Type="VI" URL="Base/Base.llb/WriteUShortScalarAttribute.vi"/>
                  <Item Name="WriteUShortSpectrumAttribute.vi" Type="VI" URL="Base/Base.llb/WriteUShortSpectrumAttribute.vi"/>
               </Item>
               <Item Name="DevicePing.vi" Type="VI" URL="Base/Base.llb/DevicePing.vi"/>
               <Item Name="DeviceStateStatus.vi" Type="VI" URL="Base/Base.llb/DeviceStateStatus.vi"/>
               <Item Name="DeviceWriteAttributes.vi" Type="VI" URL="Base/Base.llb/DeviceWriteAttributes.vi"/>
               <Item Name="DeviceReadAttributes.vi" Type="VI" URL="Base/Base.llb/DeviceReadAttributes.vi"/>
               <Item Name="DeviceExecuteCommand.vi" Type="VI" URL="Base/Base.llb/DeviceExecuteCommand.vi"/>
            </Item>
            <Item Name="AttributeValue" Type="Folder">
               <Item Name="Extractors" Type="Folder">
                  <Item Name="fromBooleanImage.vi" Type="VI" URL="Base/Base.llb/fromBooleanImage.vi"/>
                  <Item Name="fromBooleanScalar.vi" Type="VI" URL="Base/Base.llb/fromBooleanScalar.vi"/>
                  <Item Name="fromBooleanSpectrum.vi" Type="VI" URL="Base/Base.llb/fromBooleanSpectrum.vi"/>
                  <Item Name="fromDoubleImage.vi" Type="VI" URL="Base/Base.llb/fromDoubleImage.vi"/>
                  <Item Name="fromDoubleScalar.vi" Type="VI" URL="Base/Base.llb/fromDoubleScalar.vi"/>
                  <Item Name="fromDoubleSpectrum.vi" Type="VI" URL="Base/Base.llb/fromDoubleSpectrum.vi"/>
                  <Item Name="fromFloatImage.vi" Type="VI" URL="Base/Base.llb/fromFloatImage.vi"/>
                  <Item Name="fromFloatScalar.vi" Type="VI" URL="Base/Base.llb/fromFloatScalar.vi"/>
                  <Item Name="fromFloatSpectrum.vi" Type="VI" URL="Base/Base.llb/fromFloatSpectrum.vi"/>
                  <Item Name="fromLongImage.vi" Type="VI" URL="Base/Base.llb/fromLongImage.vi"/>
                  <Item Name="fromLongScalar.vi" Type="VI" URL="Base/Base.llb/fromLongScalar.vi"/>
                  <Item Name="fromLongSpectrum.vi" Type="VI" URL="Base/Base.llb/fromLongSpectrum.vi"/>
                  <Item Name="fromShortImage.vi" Type="VI" URL="Base/Base.llb/fromShortImage.vi"/>
                  <Item Name="fromShortScalar.vi" Type="VI" URL="Base/Base.llb/fromShortScalar.vi"/>
                  <Item Name="fromShortSpectrum.vi" Type="VI" URL="Base/Base.llb/fromShortSpectrum.vi"/>
                  <Item Name="fromStringScalar.vi" Type="VI" URL="Base/Base.llb/fromStringScalar.vi"/>
                  <Item Name="fromUCharImage.vi" Type="VI" URL="Base/Base.llb/fromUCharImage.vi"/>
                  <Item Name="fromUCharScalar.vi" Type="VI" URL="Base/Base.llb/fromUCharScalar.vi"/>
                  <Item Name="fromUCharSpectrum.vi" Type="VI" URL="Base/Base.llb/fromUCharSpectrum.vi"/>
                  <Item Name="fromUShortImage.vi" Type="VI" URL="Base/Base.llb/fromUShortImage.vi"/>
                  <Item Name="fromUShortScalar.vi" Type="VI" URL="Base/Base.llb/fromUShortScalar.vi"/>
                  <Item Name="fromUShortSpectrum.vi" Type="VI" URL="Base/Base.llb/fromUShortSpectrum.vi"/>
               </Item>
               <Item Name="Insertors" Type="Folder">
                  <Item Name="toBooleanImage.vi" Type="VI" URL="Base/Base.llb/toBooleanImage.vi"/>
                  <Item Name="toBooleanScalar.vi" Type="VI" URL="Base/Base.llb/toBooleanScalar.vi"/>
                  <Item Name="toBooleanSpectrum.vi" Type="VI" URL="Base/Base.llb/toBooleanSpectrum.vi"/>
                  <Item Name="toDoubleImage.vi" Type="VI" URL="Base/Base.llb/toDoubleImage.vi"/>
                  <Item Name="toDoubleScalar.vi" Type="VI" URL="Base/Base.llb/toDoubleScalar.vi"/>
                  <Item Name="toDoubleSpectrum.vi" Type="VI" URL="Base/Base.llb/toDoubleSpectrum.vi"/>
                  <Item Name="toFloatImage.vi" Type="VI" URL="Base/Base.llb/toFloatImage.vi"/>
                  <Item Name="toFloatScalar.vi" Type="VI" URL="Base/Base.llb/toFloatScalar.vi"/>
                  <Item Name="toFloatSpectrum.vi" Type="VI" URL="Base/Base.llb/toFloatSpectrum.vi"/>
                  <Item Name="toLongImage.vi" Type="VI" URL="Base/Base.llb/toLongImage.vi"/>
                  <Item Name="toLongScalar.vi" Type="VI" URL="Base/Base.llb/toLongScalar.vi"/>
                  <Item Name="toLongSpectrum.vi" Type="VI" URL="Base/Base.llb/toLongSpectrum.vi"/>
                  <Item Name="toShortImage.vi" Type="VI" URL="Base/Base.llb/toShortImage.vi"/>
                  <Item Name="toShortScalar.vi" Type="VI" URL="Base/Base.llb/toShortScalar.vi"/>
                  <Item Name="toShortSpectrum.vi" Type="VI" URL="Base/Base.llb/toShortSpectrum.vi"/>
                  <Item Name="toStringScalar.vi" Type="VI" URL="Base/Base.llb/toStringScalar.vi"/>
                  <Item Name="toUCharImage.vi" Type="VI" URL="Base/Base.llb/toUCharImage.vi"/>
                  <Item Name="toUCharScalar.vi" Type="VI" URL="Base/Base.llb/toUCharScalar.vi"/>
                  <Item Name="toUCharSpectrum.vi" Type="VI" URL="Base/Base.llb/toUCharSpectrum.vi"/>
                  <Item Name="toUShortImage.vi" Type="VI" URL="Base/Base.llb/toUShortImage.vi"/>
                  <Item Name="toUShortScalar.vi" Type="VI" URL="Base/Base.llb/toUShortScalar.vi"/>
                  <Item Name="toUShortSpectrum.vi" Type="VI" URL="Base/Base.llb/toUShortSpectrum.vi"/>
               </Item>
               <Item Name="AttributeValue.vi" Type="VI" URL="Base/Base.llb/AttributeValue.vi"/>
               <Item Name="AttributeValueRawDataCtrl.ctl" Type="VI" URL="Base/Base.llb/AttributeValueRawDataCtrl.ctl"/>
               <Item Name="AttributeValueRawDataInd.ctl" Type="VI" URL="Base/Base.llb/AttributeValueRawDataInd.ctl"/>
               <Item Name="AttributeValueDataTypeEnumInd.ctl" Type="VI" URL="Base/Base.llb/AttributeValueDataTypeEnumInd.ctl"/>
               <Item Name="AttributeValueDataTypeEnumCtrl.ctl" Type="VI" URL="Base/Base.llb/AttributeValueDataTypeEnumCtrl.ctl"/>
               <Item Name="AttributeValueCheckDataType.vi" Type="VI" URL="Base/Base.llb/AttributeValueCheckDataType.vi"/>
               <Item Name="AttributeValueDataTypeName.vi" Type="VI" URL="Base/Base.llb/AttributeValueDataTypeName.vi"/>
               <Item Name="AttributeValueExtract.vi" Type="VI" URL="Base/Base.llb/AttributeValueExtract.vi"/>
               <Item Name="AttributeValueInsert.vi" Type="VI" URL="Base/Base.llb/AttributeValueInsert.vi"/>
            </Item>
         </Item>
         <Item Name="Attribute" Type="Folder">
            <Item Name="Attribute.lvclass" Type="LVClass" URL="Attribute/Attribute.lvclass">
               <Item Name="Attribute.ctl" Type="Class Private Data" URL="Attribute/Attribute.lvclass/Attribute.ctl"/>
               <Item Name="Public" Type="Folder">
                  <Item Name="Extract.vi" Type="VI" URL="Attribute/Public/Extract.vi"/>
                  <Item Name="Identify.vi" Type="VI" URL="Attribute/Public/Identify.vi"/>
                  <Item Name="Initialize.vi" Type="VI" URL="Attribute/Public/Initialize.vi"/>
               </Item>
               <Item Name="Protected" Type="Folder">
                  <Item Name="GetAvCluster.vi" Type="VI" URL="Attribute/Protected/GetAvCluster.vi"/>
                  <Item Name="SetAvCluster.vi" Type="VI" URL="Attribute/Protected/SetAvCluster.vi"/>
               </Item>
               <Item Name="Extractors [Private]" Type="Folder">
                  <Item Name="ExtractStringScalar.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractStringScalar.vi"/>
                  <Item Name="ExtractBooleanScalar.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractBooleanScalar.vi"/>
                  <Item Name="ExtractBooleanSpectrum.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractBooleanSpectrum.vi"/>
                  <Item Name="ExtractBooleanImage.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractBooleanImage.vi"/>
                  <Item Name="ExtractUCharScalar.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractUCharScalar.vi"/>
                  <Item Name="ExtractUCharSpectrum.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractUCharSpectrum.vi"/>
                  <Item Name="ExtractUCharImage.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractUCharImage.vi"/>
                  <Item Name="ExtractShortScalar.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractShortScalar.vi"/>
                  <Item Name="ExtractShortSpectrum.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractShortSpectrum.vi"/>
                  <Item Name="ExtractShortImage.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractShortImage.vi"/>
                  <Item Name="ExtractUShortScalar.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractUShortScalar.vi"/>
                  <Item Name="ExtractUShortSpectrum.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractUShortSpectrum.vi"/>
                  <Item Name="ExtractUShortImage.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractUShortImage.vi"/>
                  <Item Name="ExtractLongScalar.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractLongScalar.vi"/>
                  <Item Name="ExtractLongSpectrum.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractLongSpectrum.vi"/>
                  <Item Name="ExtractLongImage.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractLongImage.vi"/>
                  <Item Name="ExtractFloatScalar.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractFloatScalar.vi"/>
                  <Item Name="ExtractFloatSpectrum.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractFloatSpectrum.vi"/>
                  <Item Name="ExtractFloatImage.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractFloatImage.vi"/>
                  <Item Name="ExtractDoubleImage.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractDoubleImage.vi"/>
                  <Item Name="ExtractDoubleScalar.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractDoubleScalar.vi"/>
                  <Item Name="ExtractDoubleSpectrum.vi" Type="VI" URL="Attribute/Polymorphic Imp - Extract/ExtractDoubleSpectrum.vi"/>
               </Item>
            </Item>
         </Item>
         <Item Name="Device" Type="Folder">
            <Item Name="Device.lvclass" Type="LVClass" URL="Device/Device.lvclass">
               <Item Name="Device.ctl" Type="Class Private Data" URL="Device/Device.lvclass/Device.ctl"/>
               <Item Name="Public" Type="Folder">
                  <Item Name="Identify.vi" Type="VI" URL="Device/Public/Identify.vi"/>
                  <Item Name="Ping.vi" Type="VI" URL="Device/Public/Ping.vi"/>
                  <Item Name="ReadAttribute.vi" Type="VI" URL="Device/Public/ReadAttribute.vi"/>
                  <Item Name="ReadAttributes.vi" Type="VI" URL="Device/Public/ReadAttributes.vi"/>
                  <Item Name="State.vi" Type="VI" URL="Device/Public/State.vi"/>
               </Item>
               <Item Name="Private" Type="Folder">
                  <Item Name="ReadGenericAttribute.vi" Type="VI" URL="Device/Private/ReadGenericAttribute.vi"/>
               </Item>
               <Item Name="Readers [Private]" Type="Folder">
                  <Item Name="ReadStringScalar.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadStringScalar.vi"/>
                  <Item Name="ReadBooleanScalar.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadBooleanScalar.vi"/>
                  <Item Name="ReadBooleanImage.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadBooleanImage.vi"/>
                  <Item Name="ReadBooleanSpectrum.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadBooleanSpectrum.vi"/>
                  <Item Name="ReadDoubleImage.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadDoubleImage.vi"/>
                  <Item Name="ReadDoubleScalar.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadDoubleScalar.vi"/>
                  <Item Name="ReadDoubleSpectrum.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadDoubleSpectrum.vi"/>
                  <Item Name="ReadFloatImage.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadFloatImage.vi"/>
                  <Item Name="ReadFloatScalar.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadFloatScalar.vi"/>
                  <Item Name="ReadFloatSpectrum.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadFloatSpectrum.vi"/>
                  <Item Name="ReadLongImage.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadLongImage.vi"/>
                  <Item Name="ReadLongScalar.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadLongScalar.vi"/>
                  <Item Name="ReadLongSpectrum.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadLongSpectrum.vi"/>
                  <Item Name="ReadShortImage.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadShortImage.vi"/>
                  <Item Name="ReadShortScalar.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadShortScalar.vi"/>
                  <Item Name="ReadShortSpectrum.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadShortSpectrum.vi"/>
                  <Item Name="ReadUCharImage.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadUCharImage.vi"/>
                  <Item Name="ReadUCharScalar.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadUCharScalar.vi"/>
                  <Item Name="ReadUCharSpectrum.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadUCharSpectrum.vi"/>
                  <Item Name="ReadUShortImage.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadUShortImage.vi"/>
                  <Item Name="ReadUShortScalar.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadUShortScalar.vi"/>
                  <Item Name="ReadUShortSpectrum.vi" Type="VI" URL="Device/Polymorphic Imp - ReadAttribute/ReadUShortSpectrum.vi"/>
               </Item>
            </Item>
         </Item>
         <Item Name="Examples" Type="Folder">
            <Item Name="SimplePingStateReadExample.vi" Type="VI" URL="Examples/SimplePingStateReadExample.vi"/>
            <Item Name="ReadingSeveralAttributesOnASingleDevice.vi" Type="VI" URL="Examples/ReadingSeveralAttributesOnASingleDevice.vi"/>
         </Item>
      </Item>
      <Item Name="Dépendances" Type="Dependencies"/>
      <Item Name="Spécifications de construction" Type="Build"/>
   </Item>
</Project>
