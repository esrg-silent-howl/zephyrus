<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="MPU-6050">
<description>&lt;Gyro &amp; Accelerometer 6 Axis I2C,MEMs InvenSense MPU-6050 6-Axis Motion Sensor Module&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by SamacSys&lt;/author&gt;</description>
<packages>
<package name="QFN50P400X400X95-25N">
<description>&lt;b&gt;24 Lead QFN (4x4x0.9)&lt;/b&gt;&lt;br&gt;
</description>
<smd name="1" x="-1.95" y="1.25" dx="0.8" dy="0.3" layer="1"/>
<smd name="2" x="-1.95" y="0.75" dx="0.8" dy="0.3" layer="1"/>
<smd name="3" x="-1.95" y="0.25" dx="0.8" dy="0.3" layer="1"/>
<smd name="4" x="-1.95" y="-0.25" dx="0.8" dy="0.3" layer="1"/>
<smd name="5" x="-1.95" y="-0.75" dx="0.8" dy="0.3" layer="1"/>
<smd name="6" x="-1.95" y="-1.25" dx="0.8" dy="0.3" layer="1"/>
<smd name="7" x="-1.25" y="-1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="8" x="-0.75" y="-1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="9" x="-0.25" y="-1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="10" x="0.25" y="-1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="11" x="0.75" y="-1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="12" x="1.25" y="-1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="13" x="1.95" y="-1.25" dx="0.8" dy="0.3" layer="1"/>
<smd name="14" x="1.95" y="-0.75" dx="0.8" dy="0.3" layer="1"/>
<smd name="15" x="1.95" y="-0.25" dx="0.8" dy="0.3" layer="1"/>
<smd name="16" x="1.95" y="0.25" dx="0.8" dy="0.3" layer="1"/>
<smd name="17" x="1.95" y="0.75" dx="0.8" dy="0.3" layer="1"/>
<smd name="18" x="1.95" y="1.25" dx="0.8" dy="0.3" layer="1"/>
<smd name="19" x="1.25" y="1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="20" x="0.75" y="1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="21" x="0.25" y="1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="22" x="-0.25" y="1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="23" x="-0.75" y="1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="24" x="-1.25" y="1.95" dx="0.8" dy="0.3" layer="1" rot="R90"/>
<smd name="25" x="0" y="0" dx="2.75" dy="2.65" layer="1" rot="R90"/>
<text x="0" y="0" size="1.27" layer="25" align="center">&gt;NAME</text>
<text x="0" y="0" size="1.27" layer="27" align="center">&gt;VALUE</text>
<wire x1="-2.6" y1="2.6" x2="2.6" y2="2.6" width="0.05" layer="51"/>
<wire x1="2.6" y1="2.6" x2="2.6" y2="-2.6" width="0.05" layer="51"/>
<wire x1="2.6" y1="-2.6" x2="-2.6" y2="-2.6" width="0.05" layer="51"/>
<wire x1="-2.6" y1="-2.6" x2="-2.6" y2="2.6" width="0.05" layer="51"/>
<wire x1="-2" y1="2" x2="2" y2="2" width="0.1" layer="51"/>
<wire x1="2" y1="2" x2="2" y2="-2" width="0.1" layer="51"/>
<wire x1="2" y1="-2" x2="-2" y2="-2" width="0.1" layer="51"/>
<wire x1="-2" y1="-2" x2="-2" y2="2" width="0.1" layer="51"/>
<wire x1="-2" y1="1.5" x2="-1.5" y2="2" width="0.1" layer="51"/>
<circle x="-2.35" y="2" radius="0.125" width="0.25" layer="25"/>
</package>
</packages>
<symbols>
<symbol name="MPU-6050">
<wire x1="5.08" y1="20.32" x2="25.4" y2="20.32" width="0.254" layer="94"/>
<wire x1="25.4" y1="-25.4" x2="25.4" y2="20.32" width="0.254" layer="94"/>
<wire x1="25.4" y1="-25.4" x2="5.08" y2="-25.4" width="0.254" layer="94"/>
<wire x1="5.08" y1="20.32" x2="5.08" y2="-25.4" width="0.254" layer="94"/>
<text x="26.67" y="25.4" size="1.778" layer="95" align="center-left">&gt;NAME</text>
<text x="26.67" y="22.86" size="1.778" layer="96" align="center-left">&gt;VALUE</text>
<pin name="CLKIN" x="0" y="0" length="middle"/>
<pin name="NC_1" x="0" y="-2.54" length="middle"/>
<pin name="NC_2" x="0" y="-5.08" length="middle"/>
<pin name="NC_3" x="0" y="-7.62" length="middle"/>
<pin name="NC_4" x="0" y="-10.16" length="middle"/>
<pin name="AUX_DA" x="0" y="-12.7" length="middle"/>
<pin name="AUX_CL" x="7.62" y="-30.48" length="middle" rot="R90"/>
<pin name="VLOGIC" x="10.16" y="-30.48" length="middle" rot="R90"/>
<pin name="AD0" x="12.7" y="-30.48" length="middle" rot="R90"/>
<pin name="REGOUT" x="15.24" y="-30.48" length="middle" rot="R90"/>
<pin name="FSYNC" x="17.78" y="-30.48" length="middle" rot="R90"/>
<pin name="INT" x="20.32" y="-30.48" length="middle" rot="R90"/>
<pin name="GND" x="30.48" y="0" length="middle" rot="R180"/>
<pin name="NC_8" x="30.48" y="-2.54" length="middle" rot="R180"/>
<pin name="NC_7" x="30.48" y="-5.08" length="middle" rot="R180"/>
<pin name="NC_6" x="30.48" y="-7.62" length="middle" rot="R180"/>
<pin name="NC_5" x="30.48" y="-10.16" length="middle" rot="R180"/>
<pin name="VDD" x="30.48" y="-12.7" length="middle" rot="R180"/>
<pin name="THERMAL_PAD" x="7.62" y="25.4" length="middle" rot="R270"/>
<pin name="SDA" x="10.16" y="25.4" length="middle" rot="R270"/>
<pin name="SCL" x="12.7" y="25.4" length="middle" rot="R270"/>
<pin name="RESV_3" x="15.24" y="25.4" length="middle" rot="R270"/>
<pin name="RESV_2" x="17.78" y="25.4" length="middle" rot="R270"/>
<pin name="CPOUT" x="20.32" y="25.4" length="middle" rot="R270"/>
<pin name="RESV_1" x="22.86" y="25.4" length="middle" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="MPU-6050" prefix="IC">
<description>&lt;b&gt;Gyro &amp; Accelerometer 6 Axis I2C,MEMs InvenSense MPU-6050 6-Axis Motion Sensor Module&lt;/b&gt;&lt;p&gt;
Source: &lt;a href="https://componentsearchengine.com/Datasheets/1/MPU-6050.pdf"&gt; Datasheet &lt;/a&gt;</description>
<gates>
<gate name="G$1" symbol="MPU-6050" x="0" y="0"/>
</gates>
<devices>
<device name="" package="QFN50P400X400X95-25N">
<connects>
<connect gate="G$1" pin="AD0" pad="9"/>
<connect gate="G$1" pin="AUX_CL" pad="7"/>
<connect gate="G$1" pin="AUX_DA" pad="6"/>
<connect gate="G$1" pin="CLKIN" pad="1"/>
<connect gate="G$1" pin="CPOUT" pad="20"/>
<connect gate="G$1" pin="FSYNC" pad="11"/>
<connect gate="G$1" pin="GND" pad="18"/>
<connect gate="G$1" pin="INT" pad="12"/>
<connect gate="G$1" pin="NC_1" pad="2"/>
<connect gate="G$1" pin="NC_2" pad="3"/>
<connect gate="G$1" pin="NC_3" pad="4"/>
<connect gate="G$1" pin="NC_4" pad="5"/>
<connect gate="G$1" pin="NC_5" pad="14"/>
<connect gate="G$1" pin="NC_6" pad="15"/>
<connect gate="G$1" pin="NC_7" pad="16"/>
<connect gate="G$1" pin="NC_8" pad="17"/>
<connect gate="G$1" pin="REGOUT" pad="10"/>
<connect gate="G$1" pin="RESV_1" pad="19"/>
<connect gate="G$1" pin="RESV_2" pad="21"/>
<connect gate="G$1" pin="RESV_3" pad="22"/>
<connect gate="G$1" pin="SCL" pad="23"/>
<connect gate="G$1" pin="SDA" pad="24"/>
<connect gate="G$1" pin="THERMAL_PAD" pad="25"/>
<connect gate="G$1" pin="VDD" pad="13"/>
<connect gate="G$1" pin="VLOGIC" pad="8"/>
</connects>
<technologies>
<technology name="">
<attribute name="DESCRIPTION" value="Gyro &amp; Accelerometer 6 Axis I2C,MEMs InvenSense MPU-6050 6-Axis Motion Sensor Module" constant="no"/>
<attribute name="HEIGHT" value="0.95mm" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="InvenSense Inc." constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="MPU-6050" constant="no"/>
<attribute name="MOUSER_PART_NUMBER" value="N/A" constant="no"/>
<attribute name="MOUSER_PRICE-STOCK" value="http://www.mouser.com/Search/ProductDetail.aspx?qs=u4fy%2fsgLU9O14B5JgyQFvg%3d%3d" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="MAX16150AUT+T">
<description>&lt;Supervisory Circuits Nano-Power Pushbutton ON/OFF Controller with 15kV ESD Protection and Programmable Timer&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by SamacSys&lt;/author&gt;</description>
<packages>
<package name="SOT95P280X145-6N">
<description>&lt;b&gt; 6 SOT23-6&lt;/b&gt;&lt;br&gt;
</description>
<smd name="1" x="-1.25" y="0.95" dx="1.25" dy="0.6" layer="1"/>
<smd name="2" x="-1.25" y="0" dx="1.25" dy="0.6" layer="1"/>
<smd name="3" x="-1.25" y="-0.95" dx="1.25" dy="0.6" layer="1"/>
<smd name="4" x="1.25" y="-0.95" dx="1.25" dy="0.6" layer="1"/>
<smd name="5" x="1.25" y="0" dx="1.25" dy="0.6" layer="1"/>
<smd name="6" x="1.25" y="0.95" dx="1.25" dy="0.6" layer="1"/>
<text x="0" y="0" size="1.27" layer="25" align="center">&gt;NAME</text>
<text x="0" y="0" size="1.27" layer="27" align="center">&gt;VALUE</text>
<wire x1="-2.125" y1="1.75" x2="2.125" y2="1.75" width="0.05" layer="51"/>
<wire x1="2.125" y1="1.75" x2="2.125" y2="-1.75" width="0.05" layer="51"/>
<wire x1="2.125" y1="-1.75" x2="-2.125" y2="-1.75" width="0.05" layer="51"/>
<wire x1="-2.125" y1="-1.75" x2="-2.125" y2="1.75" width="0.05" layer="51"/>
<wire x1="-0.812" y1="1.45" x2="0.812" y2="1.45" width="0.1" layer="51"/>
<wire x1="0.812" y1="1.45" x2="0.812" y2="-1.45" width="0.1" layer="51"/>
<wire x1="0.812" y1="-1.45" x2="-0.812" y2="-1.45" width="0.1" layer="51"/>
<wire x1="-0.812" y1="-1.45" x2="-0.812" y2="1.45" width="0.1" layer="51"/>
<wire x1="-0.812" y1="0.5" x2="0.138" y2="1.45" width="0.1" layer="51"/>
<wire x1="-0.275" y1="1.45" x2="0.275" y2="1.45" width="0.2" layer="21"/>
<wire x1="0.275" y1="1.45" x2="0.275" y2="-1.45" width="0.2" layer="21"/>
<wire x1="0.275" y1="-1.45" x2="-0.275" y2="-1.45" width="0.2" layer="21"/>
<wire x1="-0.275" y1="-1.45" x2="-0.275" y2="1.45" width="0.2" layer="21"/>
<wire x1="-1.875" y1="1.6" x2="-0.625" y2="1.6" width="0.2" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="MAX16150AUT+T">
<wire x1="5.08" y1="2.54" x2="25.4" y2="2.54" width="0.254" layer="94"/>
<wire x1="25.4" y1="-7.62" x2="25.4" y2="2.54" width="0.254" layer="94"/>
<wire x1="25.4" y1="-7.62" x2="5.08" y2="-7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="2.54" x2="5.08" y2="-7.62" width="0.254" layer="94"/>
<text x="26.67" y="7.62" size="1.778" layer="95" align="center-left">&gt;NAME</text>
<text x="26.67" y="5.08" size="1.778" layer="96" align="center-left">&gt;VALUE</text>
<pin name="!PB_IN" x="0" y="0" length="middle"/>
<pin name="GND" x="0" y="-2.54" length="middle"/>
<pin name="VCC" x="0" y="-5.08" length="middle"/>
<pin name="!CLR" x="30.48" y="0" length="middle" rot="R180"/>
<pin name="!INT" x="30.48" y="-2.54" length="middle" rot="R180"/>
<pin name="OUT" x="30.48" y="-5.08" length="middle" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="MAX16150AUT+T" prefix="IC">
<description>&lt;b&gt;Supervisory Circuits Nano-Power Pushbutton ON/OFF Controller with 15kV ESD Protection and Programmable Timer&lt;/b&gt;&lt;p&gt;
Source: &lt;a href=""&gt; Datasheet &lt;/a&gt;</description>
<gates>
<gate name="G$1" symbol="MAX16150AUT+T" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SOT95P280X145-6N">
<connects>
<connect gate="G$1" pin="!CLR" pad="6"/>
<connect gate="G$1" pin="!INT" pad="5"/>
<connect gate="G$1" pin="!PB_IN" pad="1"/>
<connect gate="G$1" pin="GND" pad="2"/>
<connect gate="G$1" pin="OUT" pad="4"/>
<connect gate="G$1" pin="VCC" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="DESCRIPTION" value="Supervisory Circuits Nano-Power Pushbutton ON/OFF Controller with 15kV ESD Protection and Programmable Timer" constant="no"/>
<attribute name="HEIGHT" value="1.45mm" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="Maxim Integrated" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="MAX16150AUT+T" constant="no"/>
<attribute name="MOUSER_PART_NUMBER" value="700-MAX16150AUT+T" constant="no"/>
<attribute name="MOUSER_PRICE-STOCK" value="https://www.mouser.co.uk/ProductDetail/Maxim-Integrated/MAX16150AUT%2bT?qs=wnTfsH77Xs5xR8unQygqcw%3D%3D" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="nRF24L01P-T">
<description>&lt;RF Transceiver IC 2.4GHz ISM Band QFN20 Nordic Semiconductor NRF24L01P-T, RF Transceiver 2400MHz to 2483.5MHz 1.9  3.6 V 20-Pin QFN&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by SamacSys&lt;/author&gt;</description>
<packages>
<package name="QFN50P400X400X95-21N">
<description>&lt;b&gt;Saw QFN20 (4x4 mm)&lt;/b&gt;&lt;br&gt;
</description>
<smd name="1" x="-1.95" y="1" dx="0.85" dy="0.3" layer="1"/>
<smd name="2" x="-1.95" y="0.5" dx="0.85" dy="0.3" layer="1"/>
<smd name="3" x="-1.95" y="0" dx="0.85" dy="0.3" layer="1"/>
<smd name="4" x="-1.95" y="-0.5" dx="0.85" dy="0.3" layer="1"/>
<smd name="5" x="-1.95" y="-1" dx="0.85" dy="0.3" layer="1"/>
<smd name="6" x="-1" y="-1.95" dx="0.85" dy="0.3" layer="1" rot="R90"/>
<smd name="7" x="-0.5" y="-1.95" dx="0.85" dy="0.3" layer="1" rot="R90"/>
<smd name="8" x="0" y="-1.95" dx="0.85" dy="0.3" layer="1" rot="R90"/>
<smd name="9" x="0.5" y="-1.95" dx="0.85" dy="0.3" layer="1" rot="R90"/>
<smd name="10" x="1" y="-1.95" dx="0.85" dy="0.3" layer="1" rot="R90"/>
<smd name="11" x="1.95" y="-1" dx="0.85" dy="0.3" layer="1"/>
<smd name="12" x="1.95" y="-0.5" dx="0.85" dy="0.3" layer="1"/>
<smd name="13" x="1.95" y="0" dx="0.85" dy="0.3" layer="1"/>
<smd name="14" x="1.95" y="0.5" dx="0.85" dy="0.3" layer="1"/>
<smd name="15" x="1.95" y="1" dx="0.85" dy="0.3" layer="1"/>
<smd name="16" x="1" y="1.95" dx="0.85" dy="0.3" layer="1" rot="R90"/>
<smd name="17" x="0.5" y="1.95" dx="0.85" dy="0.3" layer="1" rot="R90"/>
<smd name="18" x="0" y="1.95" dx="0.85" dy="0.3" layer="1" rot="R90"/>
<smd name="19" x="-0.5" y="1.95" dx="0.85" dy="0.3" layer="1" rot="R90"/>
<smd name="20" x="-1" y="1.95" dx="0.85" dy="0.3" layer="1" rot="R90"/>
<smd name="21" x="0" y="0" dx="2.7" dy="2.7" layer="1" rot="R90"/>
<text x="0" y="0" size="1.27" layer="25" align="center">&gt;NAME</text>
<text x="0" y="0" size="1.27" layer="27" align="center">&gt;VALUE</text>
<wire x1="-2.625" y1="2.625" x2="2.625" y2="2.625" width="0.05" layer="51"/>
<wire x1="2.625" y1="2.625" x2="2.625" y2="-2.625" width="0.05" layer="51"/>
<wire x1="2.625" y1="-2.625" x2="-2.625" y2="-2.625" width="0.05" layer="51"/>
<wire x1="-2.625" y1="-2.625" x2="-2.625" y2="2.625" width="0.05" layer="51"/>
<wire x1="-2" y1="2" x2="2" y2="2" width="0.1" layer="51"/>
<wire x1="2" y1="2" x2="2" y2="-2" width="0.1" layer="51"/>
<wire x1="2" y1="-2" x2="-2" y2="-2" width="0.1" layer="51"/>
<wire x1="-2" y1="-2" x2="-2" y2="2" width="0.1" layer="51"/>
<wire x1="-2" y1="1.5" x2="-1.5" y2="2" width="0.1" layer="51"/>
<circle x="-2.375" y="1.75" radius="0.125" width="0.25" layer="25"/>
</package>
</packages>
<symbols>
<symbol name="NRF24L01P-T">
<wire x1="5.08" y1="20.32" x2="25.4" y2="20.32" width="0.254" layer="94"/>
<wire x1="25.4" y1="-22.86" x2="25.4" y2="20.32" width="0.254" layer="94"/>
<wire x1="25.4" y1="-22.86" x2="5.08" y2="-22.86" width="0.254" layer="94"/>
<wire x1="5.08" y1="20.32" x2="5.08" y2="-22.86" width="0.254" layer="94"/>
<text x="26.67" y="25.4" size="1.778" layer="95" align="center-left">&gt;NAME</text>
<text x="26.67" y="22.86" size="1.778" layer="96" align="center-left">&gt;VALUE</text>
<pin name="CE" x="0" y="0" length="middle"/>
<pin name="CSN" x="0" y="-2.54" length="middle"/>
<pin name="SCK" x="0" y="-5.08" length="middle"/>
<pin name="MOSI" x="0" y="-7.62" length="middle"/>
<pin name="MISO" x="0" y="-10.16" length="middle"/>
<pin name="IRQ" x="10.16" y="-27.94" length="middle" rot="R90"/>
<pin name="VDD_1" x="12.7" y="-27.94" length="middle" rot="R90"/>
<pin name="VSS_1" x="15.24" y="-27.94" length="middle" rot="R90"/>
<pin name="XC2" x="17.78" y="-27.94" length="middle" rot="R90"/>
<pin name="XC1" x="20.32" y="-27.94" length="middle" rot="R90"/>
<pin name="VDD_2" x="30.48" y="0" length="middle" rot="R180"/>
<pin name="VSS_2" x="30.48" y="-2.54" length="middle" rot="R180"/>
<pin name="ANT2" x="30.48" y="-5.08" length="middle" rot="R180"/>
<pin name="ANT1" x="30.48" y="-7.62" length="middle" rot="R180"/>
<pin name="VDD_PA" x="30.48" y="-10.16" length="middle" rot="R180"/>
<pin name="THERMAL_PAD" x="7.62" y="25.4" length="middle" rot="R270"/>
<pin name="VSS_4" x="10.16" y="25.4" length="middle" rot="R270"/>
<pin name="DVDD" x="12.7" y="25.4" length="middle" rot="R270"/>
<pin name="VDD_3" x="15.24" y="25.4" length="middle" rot="R270"/>
<pin name="VSS_3" x="17.78" y="25.4" length="middle" rot="R270"/>
<pin name="IREF" x="20.32" y="25.4" length="middle" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="NRF24L01P-T" prefix="IC">
<description>&lt;b&gt;RF Transceiver IC 2.4GHz ISM Band QFN20 Nordic Semiconductor NRF24L01P-T, RF Transceiver 2400MHz to 2483.5MHz 1.9  3.6 V 20-Pin QFN&lt;/b&gt;&lt;p&gt;
Source: &lt;a href="http://www.nordicsemi.com/eng/content/download/2726/34069/file/nRF24L01P_Product_Specification_1_0.pdf"&gt; Datasheet &lt;/a&gt;</description>
<gates>
<gate name="G$1" symbol="NRF24L01P-T" x="0" y="0"/>
</gates>
<devices>
<device name="" package="QFN50P400X400X95-21N">
<connects>
<connect gate="G$1" pin="ANT1" pad="12"/>
<connect gate="G$1" pin="ANT2" pad="13"/>
<connect gate="G$1" pin="CE" pad="1"/>
<connect gate="G$1" pin="CSN" pad="2"/>
<connect gate="G$1" pin="DVDD" pad="19"/>
<connect gate="G$1" pin="IREF" pad="16"/>
<connect gate="G$1" pin="IRQ" pad="6"/>
<connect gate="G$1" pin="MISO" pad="5"/>
<connect gate="G$1" pin="MOSI" pad="4"/>
<connect gate="G$1" pin="SCK" pad="3"/>
<connect gate="G$1" pin="THERMAL_PAD" pad="21"/>
<connect gate="G$1" pin="VDD_1" pad="7"/>
<connect gate="G$1" pin="VDD_2" pad="15"/>
<connect gate="G$1" pin="VDD_3" pad="18"/>
<connect gate="G$1" pin="VDD_PA" pad="11"/>
<connect gate="G$1" pin="VSS_1" pad="8"/>
<connect gate="G$1" pin="VSS_2" pad="14"/>
<connect gate="G$1" pin="VSS_3" pad="17"/>
<connect gate="G$1" pin="VSS_4" pad="20"/>
<connect gate="G$1" pin="XC1" pad="10"/>
<connect gate="G$1" pin="XC2" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="DESCRIPTION" value="RF Transceiver IC 2.4GHz ISM Band QFN20 Nordic Semiconductor NRF24L01P-T, RF Transceiver 2400MHz to 2483.5MHz 1.9  3.6 V 20-Pin QFN" constant="no"/>
<attribute name="HEIGHT" value="0.95mm" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="Nordic Semiconductor" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="nRF24L01P-T" constant="no"/>
<attribute name="MOUSER_PART_NUMBER" value="949-NRF24L01P-T" constant="no"/>
<attribute name="MOUSER_PRICE-STOCK" value="https://www.mouser.co.uk/ProductDetail/Nordic-Semiconductor/nRF24L01P-T?qs=yd0qOhnSiDf9ybbDsku6vg%3D%3D" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="STM32F051K6U6">
<description>&lt;&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by SamacSys&lt;/author&gt;</description>
<packages>
<package name="QFN50P500X500X60-33N">
<description>&lt;b&gt;UFQFPN32 (5x5mm)_2&lt;/b&gt;&lt;br&gt;
</description>
<smd name="1" x="-2.45" y="1.75" dx="0.85" dy="0.25" layer="1"/>
<smd name="2" x="-2.45" y="1.25" dx="0.85" dy="0.25" layer="1"/>
<smd name="3" x="-2.45" y="0.75" dx="0.85" dy="0.25" layer="1"/>
<smd name="4" x="-2.45" y="0.25" dx="0.85" dy="0.25" layer="1"/>
<smd name="5" x="-2.45" y="-0.25" dx="0.85" dy="0.25" layer="1"/>
<smd name="6" x="-2.45" y="-0.75" dx="0.85" dy="0.25" layer="1"/>
<smd name="7" x="-2.45" y="-1.25" dx="0.85" dy="0.25" layer="1"/>
<smd name="8" x="-2.45" y="-1.75" dx="0.85" dy="0.25" layer="1"/>
<smd name="9" x="-1.75" y="-2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="10" x="-1.25" y="-2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="11" x="-0.75" y="-2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="12" x="-0.25" y="-2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="13" x="0.25" y="-2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="14" x="0.75" y="-2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="15" x="1.25" y="-2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="16" x="1.75" y="-2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="17" x="2.45" y="-1.75" dx="0.85" dy="0.25" layer="1"/>
<smd name="18" x="2.45" y="-1.25" dx="0.85" dy="0.25" layer="1"/>
<smd name="19" x="2.45" y="-0.75" dx="0.85" dy="0.25" layer="1"/>
<smd name="20" x="2.45" y="-0.25" dx="0.85" dy="0.25" layer="1"/>
<smd name="21" x="2.45" y="0.25" dx="0.85" dy="0.25" layer="1"/>
<smd name="22" x="2.45" y="0.75" dx="0.85" dy="0.25" layer="1"/>
<smd name="23" x="2.45" y="1.25" dx="0.85" dy="0.25" layer="1"/>
<smd name="24" x="2.45" y="1.75" dx="0.85" dy="0.25" layer="1"/>
<smd name="25" x="1.75" y="2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="26" x="1.25" y="2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="27" x="0.75" y="2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="28" x="0.25" y="2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="29" x="-0.25" y="2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="30" x="-0.75" y="2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="31" x="-1.25" y="2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="32" x="-1.75" y="2.45" dx="0.85" dy="0.25" layer="1" rot="R90"/>
<smd name="33" x="0" y="0" dx="3.6" dy="3.6" layer="1" rot="R90"/>
<text x="0" y="0" size="1.27" layer="25" align="center">&gt;NAME</text>
<text x="0" y="0" size="1.27" layer="27" align="center">&gt;VALUE</text>
<wire x1="-3.125" y1="3.125" x2="3.125" y2="3.125" width="0.05" layer="51"/>
<wire x1="3.125" y1="3.125" x2="3.125" y2="-3.125" width="0.05" layer="51"/>
<wire x1="3.125" y1="-3.125" x2="-3.125" y2="-3.125" width="0.05" layer="51"/>
<wire x1="-3.125" y1="-3.125" x2="-3.125" y2="3.125" width="0.05" layer="51"/>
<wire x1="-2.5" y1="2.5" x2="2.5" y2="2.5" width="0.1" layer="51"/>
<wire x1="2.5" y1="2.5" x2="2.5" y2="-2.5" width="0.1" layer="51"/>
<wire x1="2.5" y1="-2.5" x2="-2.5" y2="-2.5" width="0.1" layer="51"/>
<wire x1="-2.5" y1="-2.5" x2="-2.5" y2="2.5" width="0.1" layer="51"/>
<wire x1="-2.5" y1="2" x2="-2" y2="2.5" width="0.1" layer="51"/>
<circle x="-2.875" y="2.5" radius="0.125" width="0.25" layer="25"/>
</package>
</packages>
<symbols>
<symbol name="STM32F051K6U6">
<wire x1="5.08" y1="22.86" x2="35.56" y2="22.86" width="0.254" layer="94"/>
<wire x1="35.56" y1="-27.94" x2="35.56" y2="22.86" width="0.254" layer="94"/>
<wire x1="35.56" y1="-27.94" x2="5.08" y2="-27.94" width="0.254" layer="94"/>
<wire x1="5.08" y1="22.86" x2="5.08" y2="-27.94" width="0.254" layer="94"/>
<text x="36.83" y="27.94" size="1.778" layer="95" align="center-left">&gt;NAME</text>
<text x="36.83" y="25.4" size="1.778" layer="96" align="center-left">&gt;VALUE</text>
<pin name="VDD_1" x="0" y="0" length="middle" direction="pwr"/>
<pin name="PF0/OSC_IN" x="0" y="-2.54" length="middle" direction="in"/>
<pin name="PF1/OSC_OUT" x="0" y="-5.08" length="middle" direction="out"/>
<pin name="NRST" x="0" y="-7.62" length="middle" direction="in"/>
<pin name="VDDA" x="0" y="-10.16" length="middle" direction="pwr"/>
<pin name="PA0" x="0" y="-12.7" length="middle"/>
<pin name="PA1" x="0" y="-15.24" length="middle"/>
<pin name="PA2" x="0" y="-17.78" length="middle"/>
<pin name="PA3" x="10.16" y="-33.02" length="middle" rot="R90"/>
<pin name="PA4" x="12.7" y="-33.02" length="middle" rot="R90"/>
<pin name="PA5" x="15.24" y="-33.02" length="middle" rot="R90"/>
<pin name="PA6" x="17.78" y="-33.02" length="middle" rot="R90"/>
<pin name="PA7" x="20.32" y="-33.02" length="middle" rot="R90"/>
<pin name="PB0" x="22.86" y="-33.02" length="middle" rot="R90"/>
<pin name="PB1" x="25.4" y="-33.02" length="middle" rot="R90"/>
<pin name="PB2" x="27.94" y="-33.02" length="middle" rot="R90"/>
<pin name="PA14" x="40.64" y="0" length="middle" rot="R180"/>
<pin name="PA13" x="40.64" y="-2.54" length="middle" rot="R180"/>
<pin name="PA12" x="40.64" y="-5.08" length="middle" rot="R180"/>
<pin name="PA11" x="40.64" y="-7.62" length="middle" rot="R180"/>
<pin name="PA10" x="40.64" y="-10.16" length="middle" rot="R180"/>
<pin name="PA9" x="40.64" y="-12.7" length="middle" rot="R180"/>
<pin name="PA8" x="40.64" y="-15.24" length="middle" rot="R180"/>
<pin name="VDD_2" x="40.64" y="-17.78" length="middle" direction="pwr" rot="R180"/>
<pin name="VSS/VSSA_(EP)" x="10.16" y="27.94" length="middle" direction="pwr" rot="R270"/>
<pin name="PB8" x="12.7" y="27.94" length="middle" rot="R270"/>
<pin name="BOOT0" x="15.24" y="27.94" length="middle" rot="R270"/>
<pin name="PB7" x="17.78" y="27.94" length="middle" rot="R270"/>
<pin name="PB6" x="20.32" y="27.94" length="middle" rot="R270"/>
<pin name="PB5" x="22.86" y="27.94" length="middle" rot="R270"/>
<pin name="PB4" x="25.4" y="27.94" length="middle" rot="R270"/>
<pin name="PB3" x="27.94" y="27.94" length="middle" rot="R270"/>
<pin name="PA15" x="30.48" y="27.94" length="middle" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="STM32F051K6U6" prefix="IC">
<description>&lt;b&gt;&lt;/b&gt;&lt;p&gt;
Source: &lt;a href="http://www.st.com/web/en/resource/technical/document/datasheet/DM00039193.pdf"&gt; Datasheet &lt;/a&gt;</description>
<gates>
<gate name="G$1" symbol="STM32F051K6U6" x="0" y="0"/>
</gates>
<devices>
<device name="" package="QFN50P500X500X60-33N">
<connects>
<connect gate="G$1" pin="BOOT0" pad="31"/>
<connect gate="G$1" pin="NRST" pad="4"/>
<connect gate="G$1" pin="PA0" pad="6"/>
<connect gate="G$1" pin="PA1" pad="7"/>
<connect gate="G$1" pin="PA10" pad="20"/>
<connect gate="G$1" pin="PA11" pad="21"/>
<connect gate="G$1" pin="PA12" pad="22"/>
<connect gate="G$1" pin="PA13" pad="23"/>
<connect gate="G$1" pin="PA14" pad="24"/>
<connect gate="G$1" pin="PA15" pad="25"/>
<connect gate="G$1" pin="PA2" pad="8"/>
<connect gate="G$1" pin="PA3" pad="9"/>
<connect gate="G$1" pin="PA4" pad="10"/>
<connect gate="G$1" pin="PA5" pad="11"/>
<connect gate="G$1" pin="PA6" pad="12"/>
<connect gate="G$1" pin="PA7" pad="13"/>
<connect gate="G$1" pin="PA8" pad="18"/>
<connect gate="G$1" pin="PA9" pad="19"/>
<connect gate="G$1" pin="PB0" pad="14"/>
<connect gate="G$1" pin="PB1" pad="15"/>
<connect gate="G$1" pin="PB2" pad="16"/>
<connect gate="G$1" pin="PB3" pad="26"/>
<connect gate="G$1" pin="PB4" pad="27"/>
<connect gate="G$1" pin="PB5" pad="28"/>
<connect gate="G$1" pin="PB6" pad="29"/>
<connect gate="G$1" pin="PB7" pad="30"/>
<connect gate="G$1" pin="PB8" pad="32"/>
<connect gate="G$1" pin="PF0/OSC_IN" pad="2"/>
<connect gate="G$1" pin="PF1/OSC_OUT" pad="3"/>
<connect gate="G$1" pin="VDDA" pad="5"/>
<connect gate="G$1" pin="VDD_1" pad="1"/>
<connect gate="G$1" pin="VDD_2" pad="17"/>
<connect gate="G$1" pin="VSS/VSSA_(EP)" pad="33"/>
</connects>
<technologies>
<technology name="">
<attribute name="DESCRIPTION" value="" constant="no"/>
<attribute name="HEIGHT" value="0.6mm" constant="no"/>
<attribute name="MANUFACTURER_NAME" value="STMicroelectronics" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="STM32F051K6U6" constant="no"/>
<attribute name="MOUSER_PART_NUMBER" value="511-STM32F051K6U6" constant="no"/>
<attribute name="MOUSER_PRICE-STOCK" value="https://www.mouser.co.uk/ProductDetail/STMicroelectronics/STM32F051K6U6?qs=hFZnydTV6MdSevkmdmPAaA%3D%3D" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="IC1" library="MPU-6050" deviceset="MPU-6050" device=""/>
<part name="IC2" library="MAX16150AUT+T" deviceset="MAX16150AUT+T" device=""/>
<part name="IC4" library="nRF24L01P-T" deviceset="NRF24L01P-T" device=""/>
<part name="IC5" library="STM32F051K6U6" deviceset="STM32F051K6U6" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="IC1" gate="G$1" x="12.7" y="48.26" smashed="yes">
<attribute name="NAME" x="39.37" y="73.66" size="1.778" layer="95" align="center-left"/>
<attribute name="VALUE" x="39.37" y="71.12" size="1.778" layer="96" align="center-left"/>
</instance>
<instance part="IC2" gate="G$1" x="88.9" y="50.8" smashed="yes">
<attribute name="NAME" x="115.57" y="58.42" size="1.778" layer="95" align="center-left"/>
<attribute name="VALUE" x="115.57" y="55.88" size="1.778" layer="96" align="center-left"/>
</instance>
<instance part="IC4" gate="G$1" x="-25.4" y="78.74" smashed="yes">
<attribute name="NAME" x="1.27" y="104.14" size="1.778" layer="95" align="center-left"/>
<attribute name="VALUE" x="1.27" y="101.6" size="1.778" layer="96" align="center-left"/>
</instance>
<instance part="IC5" gate="G$1" x="142.24" y="91.44" smashed="yes">
<attribute name="NAME" x="179.07" y="119.38" size="1.778" layer="95" align="center-left"/>
<attribute name="VALUE" x="179.07" y="116.84" size="1.778" layer="96" align="center-left"/>
</instance>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
