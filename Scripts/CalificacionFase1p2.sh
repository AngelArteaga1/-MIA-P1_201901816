/*Debería generar los ids: vda1, vdb1, vda2, vdb2, vda3, vdb3*/
/*vda para el D3.dsk y vdb para d1.dsk*/
mouNt $NamE::"PRI 1"  $path::"/home/archivos/fase 1/D3.dsk"
mouNt $path::"/home/archivos/fase 1/D1.dsk" $NamE::"PRI 1"
moUnt $PAth::"/home/archivos/fase 1/D3.dsk" $name::"PRI 2"
moUnt $name::"PRI 2" $PAth::"/home/archivos/fase 1/D1.dsk"
mOunt $name::"LOG 1" $path::"/home/archivos/fase 1/D3.dsk"
mouNt $path::"/home/archivos/fase 1/D1.dsk" $name::"LOG1"
mount
/*Crea el sistema de archivos EXT3 en todas las particiones primarias PRI1*/ 
mkfs $id=>vda1 @type=>Full 
Mkfs $id=>vdb1 
/*Crea el sistema de archivos EXT3 en todas las particiones Logicas*/
mkfs $id=>vda3 @type=>Full 
Mkfs $id=>vdb3 @type=>FasT
/*Crearemos la ruta y las padres que no existan
mkdir $id=>vda1 $path=>"/home/user/mia/fase1" @p
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta2" @p
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta3/carpeta7/carpeta8/carpeta9/carpeta10/carpeta11" @p
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta4/carpeta7/carpeta8/carpeta9/carpeta10/carpeta11 \
/carpeta7/carpeta8/carpeta9/carpeta10/carpeta11" @p
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta2/a1"
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta2/a2"
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta2/a3"
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta2/a4"
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta2/a5"
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta2/a6"
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta2/a7"
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta2/a8"
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta2/a9"
mkdir $id=>vda1 $path=>"/home/user/mia/carpeta2/a10"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a1"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a2"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a3"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a4"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a5"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a6"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a7"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a8"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a9"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a10"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a11"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a12"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a13"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a14"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a15"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a16"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a17"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a18"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a19"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a20"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a21"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a22"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a23"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a24"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a25"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a26"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a27"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a28"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a29"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a30"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a31"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a32"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a33"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a34"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a35"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a36"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a37"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a38"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a39"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a40"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a41"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a42"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a43"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a44"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a45"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a46"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a47"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a48"

/*Primer indirecto
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b1"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b2"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b3"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b4"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b5"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b6"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b7"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b8"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b9"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b10"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b11"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b12"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b13"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b14"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b15"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b16"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b17"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b18"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b19"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b20"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b21"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b22"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b23"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b24"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b25"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b26"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b27"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b28"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b29"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b30"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b31"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b32"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b33"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b34"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b35"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b36"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b37"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b38"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b39"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b40"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b41"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b42"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b43"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b44"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b45"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b46"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b47"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b48"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b49"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b50"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b51"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b52"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b53"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b54"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b55"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b56"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b57"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b58"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b59"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b60"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b61"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b62"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b63"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b64"

/*Segundo indirecto*/
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c1"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c2"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c3"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c4"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c5"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c6"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c7"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c8"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c9"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c10"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c11"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c12"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c13"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c14"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c15"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c16"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c17"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c18"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c19"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c20"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c21"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c22"


/*Validar que no cree dos veces el mismo directorio*/
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/c16"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a10"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/a12"
mkdir $id=>vda1 $path=>"/home/user/mia/fase1/b60"



rep $nAmE=>bm_inode $Path=>"/home/archivos/reportes/bm_inode01.jpg" $id=>vda1
rep $NaMe=>bm_block $Path=>"/home/archivos/reportes/bm_block01.jpg" $id=>vda1
rep $NAMe=>sb $patH=>"/home/archivos/reportes/sb01.txt" $id=>vda1
rep $NamE=>bitacora $patH=>"/home/archivos/reportes/bitacora01.jpg" $id=>vda1
rep $NAME=>tree $patH=>"/home/archivos/reportes/directorio01UsoApuntadoresCarpetas.jpg" $id=>vda1
/*uso de bloques e inodos*/


/*Archivo*/ 
mkfile $id=>vda3 $path=>"/home/b1.txt" $size=>767
/*Archivo*/
mkfile $id=>vda3 $path=>"/home/a1.txt" $size=>767

/*Crearemos la ruta y las padres que no existan
mkdir $id=>vda3 $path=>"/home/user/mia/fase1" @p
mkdir $id=>vda3 $path=>"/home/user/mia/carpeta2" @p
mkdir $id=>vda3 $path=>"/home/user/mia/carpeta3" @p
mkdir $id=>vda3 $path=>"/home/user/mia/carpeta4" @p


/*Archivo*/
mkfile $id=>vda3 $path=>"/home/user/mia/fase1/fase1.txt" $size=>18175
/*Archivo*/
mkfile $id=>vda3 $path=>"/home/user/mia/fase1/a1.txt" $size=>767
/*Archivo*/
/*Utiliza */
mkfile $id=>vda3 $path=>"/home/user/mia/fase1/a2.txt" $size=>1791
/*Archivo */
/*Utiliza */
mkfile $id=>vda3 $path=>"/home/user/mia/fase1/a3.txt" $size=>18175
/*Archivo */
/*Utiliza */
mkfile $id=>vda3 $path=>"/home/user/mia/fase1/a4.txt" $size=>280319
/*Archivo */
/*Utiliza n cantidad bloques aproximadamente*/
mkfile $id=>vda3 $path=>"/home/user/mia/fase1/a5.txt" $size=>280319

/*generamos el reporte tree para ver el uso de apuntadores*/
rep $NamE=>tree $Path=>"/home/user/reportes/treeUsoApuntadoresArchivos03.png" $id=>vda3
/*uso de bloques e inodos*/
rep $NamE=>inode $Path=>"/home/user/reportes/inodeArchivos03.png" $id=>vda3
rep $NamE=>block $Path=>"/home/user/reportes/blockArchivos03.png" $id=>vda3

/*Crear archivo con contenido, pegar fase1.txt en la carpeta archivos*/
mkfile $id=>vda3 $path=>"/home/user/mia/fase1/a6.txt" 
/*vamos a ver el archivo user.txt en el reporte file*/
rep $NamE=>file $Path=>"file02A6.txt" $id=>vda1 @ruta=>"/home/user/mia/fase1/a6.txt"


/*Parametro para eliminar archivos*/
rm $FileID=>a1.txt $Id=>vdb1
rm $FileID=>a5.txt $Id=>vdb1
rm $FileID=>b1.txt $Id=>vdb1 
/*generamos el reporte tree para ver el uso de apuntadores*/
rep $name=>tree $Path=>"/home/user/reportes/tree05reporteRM.png" $id=>vda3


/*CP*/
/*copiar archivo a3 a carpeta 3*/
cp $id=>vda1 $Path=>"/home/user/mia/fase1/a3.txt" $iddest=>vda1 $dest=>"/home/user/mia/carpeta3"

/*copiar carpeta3 a Carpeta2*/
cp $id=>vda1 $Path=>"/home/user/mia/carpeta3" $iddest=>vda1 $dest=>"/home/user/mia/carpeta2"

/*copiar fase1 a Carpeta3*/
cp $id=>vda1 $Path=>"/home/user/mia/fase1" $iddest=>vda1 $dest=>"/home/user/mia/carpeta3"

rep $NAME=>tree $Path=>"/home/user/reportes/treecopy1.png" $id=>vda1

/*MV*/
mv $id=>vda1 $iddest=>vda1 $Path=>"/home/user/mia/carpeta3/fase1" $dest=>"/home/user/mia/carpeta4"
rep $NAME=>tree $Path=>"/home/user/reportes/treemove1.png" $id=>vda1





/*FIND*/
find $id=>vda1 $Path=>"/" $NaMe=>*
find $id=>vda1 $Path=>"/home" $NaMe=>"?.*"



/*REPORTE JOUTNALING*/
rep $name=>bitacora $patH=>"/home/archivos/reportes/bitacora02.txt" $id=>vda1
rep $nAme=>bitacora $patH=>"/home/archivos/reportes/bitacora03.txt" $id=>vda3


/*FALLO EN EL SISTEMA Y RECUPERACION DE SISTEMA DE ARCHIVOS*/
/*Simulando la perdida del sistema de archivos EXT3 de la partición1*/
Loss $id=>vda1

rep $name=>bm_inode $Path=>"/home/archivos/reportes/bm_inodeLOSS01.txt" $id=>vda1
rep $name=>bm_block $Path=>"/home/archivos/reportes/bm_blockLOSS01.txt" $id=>vda1
rep $NaMe=>sb $patH=>"/home/archivos/reportes/sb01.txt" $id=>vda1
rep $Name=>journaling $patH=>"/home/archivos/reportes/bitacoraLOSS01.txt" $id=>vda1
rep $name=>tree $patH=>"/home/archivos/reportes/directorioLOSS01.txt" $id=>vda1
/*uso de bloques e inodos*/

Recovery $id=>vda1

rep $name=>bm_inode $Path=>"/home/archivos/reportes/bm_inodeLOSS01.txt" $id=>vda1
rep $name=>bm_block $Path=>"/home/archivos/reportes/bm_blockLOSS01.txt" $id=>vda1
rep $NaMe=>sb $patH=>"/home/archivos/reportes/sb01.txt" $id=>vda1
rep $Name=>journaling $patH=>"/home/archivos/reportes/bitacoraLOSS01.txt" $id=>vda1
rep $name=>tree $patH=>"/home/archivos/reportes/directorioLOSS01.txt" $id=>vda1
/*uso de bloques e inodos*/











