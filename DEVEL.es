1. Introducci�n
---------------

En este fichero encontrar�s los detalles de la implementaci�n e informaci�n
general para desarrolladores que quieran saber como trabaja meshias
internamente.

Primeramente queremos implementar AODV, y luego ir a�adiendo alunos cambios
para mejorarlo. AODV es un estardar para Adoc On-Demand Distance Vector
Routing, y es un protocolo para convertir una red Ad-hoc en una red
mesh, en la cual cada nodo no tiene porque estar conectada directamente al nodo
con el que queremos comunicarnos, por ello necesitamos crear una camino por
nodos intermedios hasta llegar al objetivo.


2. Librerias
------------

Para empezar con este proyecto necesitamos conocer que estamos usando y por 
qu�:

 * libnetfilter_queue
 Para capturar y filtrar paquetes. Debemos capturar todos los paquetes que
 salgan por nuestra intefaz y determinar si su ruta es conocida o no. En
 caso de que no sea conocida, capturamos el paquete y lo guardamos en
 un buffer. Adem�s mandamos un paquete AODV en busqueda de la ruta pedida.
 Si nos llega una respuesta de AODV con la ruta, la a�adimos a la tabla de
 rutas y dejamos que los paquetes antes capturados sigan su camino. Aclarar
 que los paquetes realmente los enruta el n�cleo, lo �nico que hacemos
 nosotros es a�adir esta ruta.

 * libnl,
 La comunicaci�n con el kernel para las rutas lo hacemos con esta libreria,
 ya sea obtener la tabla, actualizar o modificarla. En este caso usaremos libnl
 en ver de usar directamente netlink porque �ste es mucho m�s a bajo nivel
 para lo que realmente necesitamos.

 * udp sockets
 Para enviar paquetes AODV para el manejo de las rutas se hace a trav�s del
 puerto 654 UDP.

 * unix sockets,
 Para la comunicacion entre el demonio y sus herramientas.

 * conntrack alarms
 Para el manejo de los "timeout" del protocolo AODV.

3. AODV
-------

El protocolo AODV es descrito en el RFC 3561. Route Requests (RREQs), Route
Replies (RREPs) and Route Errors (RERRs) son los tipos de mensajes que existen
en AODV. Estos tipos de paquetes son enviados por UDP en el puerto 654.

tatatata
