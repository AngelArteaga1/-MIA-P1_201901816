/*crearemos 8 discos...*/
/*verificar tamaño de discos...*/
MKdisk $tamaNio=>120 $path=>"/home/archivos/fase 1/" $nOMbRe=>"D1.dsk"
mkdisK $taMaNio=>32 $path=>"/home/archivos/fase 1/" $nOMbRe=>"D2.dsk" 
MKdisk $tamaNio=>64 $PatH=>"/home/archivos/fase 1/" $nOMbRe=>"D3.dsk"
MKdisk $tamaNio=>80 $path=>"/home/archivos/fase 1/" $nOMbRe=>"D4.dsk" 
MKdisk $tAmanio=>120 $path=>"/home/archivos/fase 1/" $nOMbRe=>"D5.dsk"
MKdisk $TamaNio=>8 $path=>"/home/archivos/fase 1/" $nOMbRe=>"D6.dsk"

/*vamos a verificar si hicieron validaciones con datos incorrectos*/
/*primero el tamaNio tiene numero menor a 0, segundo el 60 no es multiplo de 8*/
MKdisk $tamaNio=>-60 $path=>"/home/archivos/fase 1/" $nOMbRe=>"D7.dsk"
MKdisk $tamaNio=>60 $path=>"/home/archivos/fase 1/" $nOMbRe=>"D8.dsk"

/*Eliminar discos, solo quedaran 3 discos*/
/*Verificacion de mensaje de confirmacion de eliminacion de discos*/
RMdisk $patH=>"/home/archivos/fase 1/D5.dsk"
RMdisk $patH=>"/home/archivos/fase 1/D4.dsk"

/*Verificando validaciones con datos incorrectos en el rmdisk*/
/*Deberia de dar error ya que el disco 8 no existe*/
rmDiSK $path=>"/home/archivos/fase 1/D8.dsk"

/*Crear 12 particiones del mismo tamaño en el primer disco con ajustes diferentes*/
/*PrimarIA 1 es de 10 Mb*/
/*Todas son de 5 Mb (La extendida es de 50 Mb)*/
/*Tipo:		Primaria	Primaria	Extendida	Logica	Logica	Logica	Logica	Logica	Logica	Logica	Logica Primaria*/
/*Ajuste:	Primer 		Mejor		$			Peor	Peor	Peor	Peor	Peor	Peor	Peor	Mejor   Peor*/
/*Nombre:	PRI 1		PRI 2		EXT			LOG 1	LOG 2	LOG 3	LOG 4	LOG 5	LOG 6	LOG 7	LOG8   PRI 3*/
fdisk $tamaNio=>10 @UniT=>M $path=>"/home/archivos/fase 1/D1.dsk"    @type=>P @fit=>FF $nOMbRe=>"PRI 1"
fdisk $path=>"/home/archivos/fase 1/D1.dsk" $tamaNio=>5120 	  @fit=>BF $nOMbRe=>"PRI 2"
fdisk $path=>"/home/archivos/fase 1/D1.dsk" @type=>E $nOMbRe=>"EXT" $tamaNio=>51200
fdisk @type=>L $tamaNio=>5120 @Unit=>K $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"LOG 1"
fdisK $nOMbRe=>"LOG 2" $tamaNio=>5120 $path=>"/home/archivos/fase 1/D1.dsk" @type=>L
fdisk @type=>L $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"LOG 3" $tamaNio=>5120 @uniT=>K
fdiSk @type=>L $tamaNio=>5120 @unIt=>K $PAth=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"LOG 4"
fdisk @type=>L $tamaNio=>5120 @unit=>K $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"LOG 5"
FdisK $tamaNio=>5120 $nOMbRe=>"LOG 6" @UNit=>K $path=>"/home/archivos/fase 1/D1.dsk" @type=>L
fdIsk @type=>L $tamaNio=>5120 @unit=>K $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"LOG 7"
fdIsk @type=>L $tamaNio=>5120 @unit=>K $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"LOG 8" @fit=>BF
fdisk @unit=>B $path=>"/home/archivos/fase 1/D1.dsk" $tamaNio=>5242880 $nOMbRe=>"PRI 3"

/*Crearemos las mismas 12 particiones en el tercer disco*/
/*PrimarIA 1 es de 10 Mb*/
/*eL RESTO son de 5 Mb (La extendida es de 50 Mb)*/
/*Tipo:		Primaria	Primaria	Extendida	Logica	Logica	Logica	Logica	Logica	Logica	Logica	Logica Primaria*/
/*Ajuste:	Primer 		Mejor		$			Peor	Peor	Peor	Peor	Peor	Peor	Peor	Mejor   Peor*/
/*Nombre:	PRI 1		PRI 2		EXT			LOG 1	LOG 2	LOG 3	LOG 4	LOG 5	LOG 6	LOG 7	LOG8   PRI 3*/

fdisk $tamaNio=>10 @UniT=>M $path=>"/home/archivos/fase 1/D3.dsk"     @type=>P @fit=>FF $nOMbRe=>"PRI 1"
fdisk $path=>"/home/archivos/fase 1/D3.dsk" $tamaNio=>5120 	 @fit=>BF $nOMbRe=>"PRI 2"
FdisK $path=>"/home/archivos/fase 1/D3.dsk" @type=>E $nOMbRe=>"EXT" $tamaNio=>51200
fdisk @type=>L $tamaNio=>5120 @Unit=>K $path=>"/home/archivos/fase 1/D3.dsk" $nOMbRe=>"LOG 1"
fdisK $nOMbRe=>"LOG 2" $tamaNio=>5120 $path=>"/home/archivos/fase 1/D3.dsk" @type=>L
fdisk @type=>L $path=>"/home/archivos/fase 1/D3.dsk" $nOMbRe=>"LOG 3" $tamaNio=>5120 @uniT=>K
fdiSk $tamaNio=>5120 @unIt=>K $PAth=>"/home/archivos/fase 1/D3.dsk" $nOMbRe=>"LOG 4" @type=>L
fdisk @type=>L $tamaNio=>5120 @unit=>K $path=>"/home/archivos/fase 1/D3.dsk" $nOMbRe=>"LOG 5"
FdisK $tamaNio=>5120 $nOMbRe=>"LOG 6" @UNit=>K $path=>"/home/archivos/fase 1/D3.dsk" @type=>L
fdIsk @type=>L $tamaNio=>5120 @unit=>K $path=>"/home/archivos/fase 1/D3.dsk" $nOMbRe=>"LOG 7"
fdIsk @type=>L $tamaNio=>5120 @unit=>K $path=>"/home/archivos/fase 1/D3.dsk" $nOMbRe=>"LOG 8" @fit=>BF
fdisk @unit=>B $path=>"/home/archivos/fase 1/D3.dsk" $tamaNio=>5242880 $nOMbRe=>"PRI 3"

/*Verificar generacion de ids al montar*/
/*Debería generar los ids: vda1, vdb1, vda2, vdb2*/
/*vda para el d3.dsk y vdb para d1.dsk*/
mouNt $nOMbRe=>"PRI 3"  $path=>"/home/archivos/fase 1/D3.dsk"
moUnt $PAth=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"PRI 3"
mOunt $nOMbRe=>"PRI 2" $path=>"/home/archivos/fase 1/D3.dsk"
mouNt $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"PRI 2"

/*Validaciones al montar, no deberia cargarlos*/
moUnt $nOMbRe=>PX $path=>"/home/archivos/fase 1/D3.dsk"
moUNt $path=>"/home/archivos/fase 1/DX.dsk" $nOMbRe=>"PRI 3"

/*Listar las particiones montadas*/
mouNt
/*SAlida es la siguiente*/
/*id=>vda1 $path=>"/home/archivos/fase 1/D3.dsk" $nOMbRe=>"PRI 3"*/
/*id=>vdb1 $PAth=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"PRI 3"*/
/*id=>vda2 $path=>"/home/archivos/fase 1/D3.dsk" $nOMbRe=>"PRI 2"*/
/*id=>vdb2 $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"PRI 2"*/

/*Reporte de las particiones del disco*/
/*Debe crear la carpeta de reportes*/
rep $nOMbRe=>"disk" $path=>"/home/archivos/fase 1/reportes/Reporte1_Disco_Disco3.jpg" $id=>vda1
rep $path=>"/home/archivos/fase 1/reportes/Reporte1_disco_Disco1.jpg" $id=>vdb1 $nOMbRe=>"disk"
rep $nOMbRe=>"mbr" $path=>"/home/archivos/fase 1/reportes/Reporte1_MBR_Disco3.jpg" $id=>vda1
rep $path=>"/home/archivos/fase 1/reportes/Reporte1_MBR_Disco1.jpg" $id=>vdb1 $nOMbRe=>"mbr"

/*Vericamos la eliminación*/
/*Elimina la primaria 2, logica 1, logica 2 y logica 5 del disco 1*/
/*Queda así: P1 EXT L2 L3 L6 L7 L8 P3*/
fdisk @delete=>Fast $nOMbRe=>"PRI 2" $path=>"/home/archivos/fase 1/D1.dsk"
fdisk $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"LOG 1" @delete=>FasT
fDisk $nOMbRe=>"LOG 4" @delete=>FaST $PAth=>"/home/archivos/fase 1/D1.dsk"
fdisK $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"LOG 5" @delete=>FAsT

/*Tambien elimina la partición extendida y la primaria 1 del disco 3*/
/*Verificar que no queden EBRs*/
/*Queda así: P2 P3*/
fDisk $DELete=>Full $nOMbRe=>"EXT" $path=>"/home/archivos/fase 1/D3.dsk" @Delete=>Full
fdisk @Delete=>Full $nOMbRe=>"PRI 1" @Delete=>Full $path=>"/home/archivos/fase 1/D3.dsk"

/*Reporte con particiones eliminadas*/
rep $id=>vda1 $nOMbRe=>"disk" $path=>"/home/archivos/fase 1/reportes/Reporte2_disco_Disco3.jpg"
rep $path=>"/home/archivos/fase 1/reportes/Reporte2_disco_Disco1.jpg" $nOMbRe=>"disk" $id=>vdb1
rep $nOMbRe=>"mbr" $path=>"/home/archivos/fase 1/reportes/Reporte2_MBR_Disco3.jpg" $id=>vda1
rep $path=>"/home/archivos/fase 1/reportes/Reporte2_MBR_Disco1.jpg" $id=>vdb1 $nOMbRe=>"mbr"

/*TEORIA DE PARTICIONES, (solo una extendida) debería dar error*/
fdisk $tamaNio=>2048 $path=>"/home/archivos/fase 1/D1.dsk" @type=>E $nOMbRe=>"EXT2"

/*Verificar PRIMER AJUSTE creando de nuevo las particiones*/
/*Debe quedar de la siguiente forma:*/
/*Disco1: PRIMARIA1 PRIMARIA2 EXTENDIDA LOGICA1 LOGICA2 LOGICA3 LOGICA45 LOGICA6 LOGICA7 LOGICA8 PRIMARIA3*/
fdiSk @unit=>M $tamaNio=>5 $path=>"/home/archivos/fase 1/D1.dsk" @type=>P @fit=>FF $nOMbRe=>"PRI 2"
fdisk $path=>"/home/archivos/fase 1/D1.dsk" $tamaNio=>10240 @type=>L $nOMbRe=>"LOG 45" @unit=>K
fdisk $path=>"/home/archivos/fase 1/D1.dsk" $tamaNio=>5120 @type=>L $nOMbRe=>"LOG 1" @unit=>K


/*Disco 3: Primaria2 EXTENDIDA LOGICA1 PRIMARIA1 PRIMARIA3*/
fdisk @type=>E $path=>"/home/archivos/fase 1/D3.dsk" $tamaNio=>10240 $nOMbRe=>"EXT"
fdiSk $nOMbRe=>"LOG 1" @unit=>K $path=>"/home/archivos/fase 1/D3.dsk" @type=>L $tamaNio=>5120
fdisk $tamaNio=>10240 $nOMbRe=>"PRI 1" $path=>"/home/archivos/fase 1/D3.dsk" @type=>P

/*Reporte de las particiones del disco*/
Rep $Path=>"/home/archivos/fase 1/reportes/Reporte3_disco_Disco3.jpg" $id=>vda1 $nOMbRe=>"disk"
rep $nOMbRe=>"disk" $path=>"/home/archivos/fase 1/reportes/Reporte3_disco_Disco2.jpg" $id=>vdb1

/*Validaciones al crear o quitar espacio*/
/*No deberia ejecutar estas instrucciones*/
/*(porque no hay espacio después)*/
fdisk $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"PRI 1" @add=>3
/*(porque no debe quedar espacio negativo) debe mostrar mensaje de cuanto puede quitarle*/
fdisk @add=>-20 @unit=>M $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"PRI 1" @unit=>M
/*(Porque no hay suficiente espacio)*/
fdisk @unit=>M $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"LOG 7" @add=>150

/*Quita y agrega 1 mb de la primer particion*/
fdisk @unit=>M $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"PRI 1" @add=>-1
rep $nOMbRe=>"Mbr" $path=>"/home/archivos/fase 1/reportes/Reporte4_MBR_Espacio_Disco1.jpg" $id=>vdb2
fdisk @unit=>M $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"PRI 1" @add=>1
rep $nOMbRe=>"Mbr" $path=>"/home/archivos/fase 1/reportes/Reporte5_MBR_Espacio_Disco1.jpg" $id=>vdb2
/*Le quita 1 mb a la partición logica*/
fdisk @unit=>M $path=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"LOG 45" @add=>$1
rep $nOMbRe=>"Mbr" $path=>"/home/archivos/fase 1/reportes/Reporte6_Espacio_MBR Disco1.jpg" $id=>vdb2

/*desmontar una lista de ids
umount $id1=>vda1 $id2=>vdb2 $id=>vda2
/*para corroborar que desmonto hacer un mount para listar las particiones montadas*/
mount
/*Salida es la siguiente*/
/*id=>vdb1 $PAth=>"/home/archivos/fase 1/D1.dsk" $nOMbRe=>"PRI 3"*/