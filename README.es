1. Introducción
---------------

Meshias es una implementación del protocolo de enrutado AODV para redes mesh.
Meshias captura paquetes IP salientes de nuestro equipo, y los acepta si ya
conoce una ruta para la IP destino, o los "roba" temporalmente mientras busca
una ruta. Está pensado para funcionar en redes wifi pero funciona en cualquier
tipo de red IP. Meshias es transparente para todos los programas que funcionen
en la capa de red de IP o superiores.


2. Compilación e instalación
----------------------------

Meshias todavía no es muy estable por tanto la única manera de usarlo es
compilarlo e instalarlo. Para compilar meshias hace falta una versión reciente 
del kernel de linux. Meshias utiliza herramientas del núcleo de linux como son
netfilter o libnl por tanto sólo funcionará en este sistema operativo.

Lo primero que debemos hacer es descargarnos el código del repositorio de 
desarrollo. Usamos bazaar como sistema de control de versiones, debido a los 
errores que tuvimos con la forja de RedIRIS, que aunque subimos el código no
siempre será el más aactual. El siguiente comando hay que ejecutarlo con
permisos de superusuario para instalarlo:

    apt-get install bzr

Y para descargarnos el código:

    bzr branch lp:meshias

Si usas Debian o un derivado como Ubuntu, el comando a ejecutar como
superusuario para instalar estas dependencias es:

    apt-get install libnl-dev libnfnetlink-dev cmake build-essential

El siguiente paso es compilar meshias. Para ello entramos en el directorio donde
está el código, y lo compilamos:

    cd meshias/src/
    cmake .
    make

Finalmente para instalar meshias en el sistema:

    sudo make install

3. Uso
------

Ahora vamos a configurar la red a mano porque por ahora meshias no tiene ninguna
utilidad para hacerlo de forma más sencilla. Si usamos algún demonio de red
que intenta automática conectarse a redes wifi disponibles, tenemos que
deshabilitarlo para que no deshaga lo que nosotros configuremos. Probablemente
en tu distribución uses el Network Manager, en cuyo caso debes ejecutar como
superusuario:

  /etc/init.d/NetworkManager stop

Para hacer funcionar a meshias, tenemos que añadir varias reglas de iptables
(todas hay que ejecutarlas como superusuario). Sin embargo, lo primero que
necesitamos es poner nuestra tarjeta wireless en modo Ad-Hoc. Si tenemos una
tarjeta con chipset Atheros, esto se hace con los siguientes comandos:

    wlanconfig ath0 destroy
    wlanconfig ath0 create wlandev wifi0 wlanmode Ad-Hoc

En otras tarjetas wireless para poner la interfaz de red wifi en modo ad-hoc
se hace simplemente con el siguiente comando:

    iwconfig wlan0 mode Ad-Hoc

En las tarjetas atheros la interfaz de red wifi se llama ath0, en otras wlan0,
y en otras ra0 etc. En este tutorial, en lo que sigue supondremos que nuestra
interfaz se llama ath0.

Una vez hecho esto si ha funcionado correctamente, el comando iwconfig ath0 debe
de sacar en su salida que el modo en el que está la tarjeta es Ad-Hoc.

    iwconfig ath0
    ath0      IEEE 802.11g  ESSID:""  Nickname:""
              Mode:Ad-Hoc Channel:0  Access Point: Not-Associated
              Bit Rate:0 kb/s   Tx-Power:16 dBm   Sensitivity=1/1
              Retry:off   RTS thr:off   Fragment thr:off
              Encryption key:off
              Power Management:off
              Link Quality=0/70  Signal level=-96 dBm  Noise level=-96 dBm
              Rx invalid nwid:1836  Rx invalid crypt:0  Rx invalid frag:0
              Tx excessive retries:0  Invalid misc:0   Missed beacon:0
            
Ahora que tenemos la interfaz en modo Ad-Hoc, necesitamos configurar en qué red
wifi vamos a estar. Por ejemplo, usaremos una cuyo ESSID es "meshias" y está
en el canal 6:

    iwconfig ath0 essid meshias ch 6
  
Podremos ver que está configurado correctamente si vemos que la salida de
iwconfig algo parecido a esta:

    
    iwconfig ath0
    ath0      IEEE 802.11g  ESSID:"meshias"  Nickname:""
              Mode:Ad-Hoc  Frequency:2.437 GHz  Cell: DE:AD:BE:EF:DE:AD
              Bit Rate:0 kb/s   Tx-Power:9 dBm   Sensitivity=1/1
              Retry:off   RTS thr:off   Fragment thr:off
              Encryption key:off
              Power Management:off
              Link Quality=81/70  Signal level=0 dBm  Noise level=-72 dBm
              Rx invalid nwid:0  Rx invalid crypt:0  Rx invalid frag:0
              Tx excessive retries:0  Invalid misc:0   Missed beacon:0

Podemos comprobar que a diferencia de antes, tenemos essid, la frecuencia marca
2.437Ghz (que coresponde al canal 6), y Cell ya no marca "Not-Associated".
Además Link Quality no es cero. Todo esto es buena señal y significa que vamos
bien.

Entonces ya tenemos creada la interfaz de red que vamos usar  para la red mesh, y
está en el modo correcto, pero ahora hace falta asignarnos una dirección ip
y un rango de red. AODV no soporta el uso de servidores de DHCP ni routers que den
acceso a internet, eso es algo que en Meshias una vez implementado el protocolo,
nos  dedicaremos a implementarlo extendiendo el protocolo. Pero por ahora
tendremos que utilizar IPs locales de nuestra red y sin servidores de nombres.

Con el siguiente comando asignamos la ip 192.168.0.2 y el broadcast
255.255.255.0 a la interfaz de red y la levantamos:

    ifconfig ath0 192.168.0.2 netmask 255.255.255.0 up

Podemos comprobar que funcionó correctamente ejecutando el comando ifconfig ath0
que efectivamente tenemos esa ip y máscara de red asignadas:

    ifconfig ath0
    ath0      Link encap:Ethernet  HWaddr de:ad:be:ef:de:ad:be
            inet addr:192.168.6.2  Bcast:192.168.6.255  Mask:255.255.255.0
            inet6 addr: fe80::223:4eff:fe3e:2851/64 Scope:Link
            UP BROADCAST MULTICAST  MTU:1500  Metric:1
            RX packets:0 errors:0 dropped:0 overruns:0 frame:0
            TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
            collisions:0 txqueuelen:0
            RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

Cuando levantamos la interfaz, debería también haberse agregado una ruta a la
tabla de rutas del kernel que diga que las rutas 192.168.0.X van por la interfaz
ath0. La salida del comando route -n debería de mostrar esa ruta:

    route -n
    Kernel IP routing table
    Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
    192.168.0.0     0.0.0.0         255.255.255.0   U     0      0        0 ath0

Finalmente necesitamos que meshias pueda capturar los paquetes que enviemos a
esa red wifi a la que nos hemos conectado. Los comandos son los siguientes, y
hay que ejecutarlo en orden:

    iptables -t filter -A OUTPUT -o eth0 -p udp --dport 8765 -j DROP
    iptables -t filter -A INPUT -i eth0 -p udp --dport 8765 -j DROP
    iptables -t filter -A OUTPUT -o eth0 -p udp --sport 654 -j ACCEPT
    iptables -t filter -A INPUT -i eth0 -p udp --dport 654 -j ACCEPT
    iptables -t filter -A OUTPUT -o eth0 -j NFQUEUE --queue-num 0
    iptables -t filter -A INPUT -i eth0 -j NFQUEUE --queue-num 0
    iptables -t filter -A FORWARD -i eth0 -j NFQUEUE --queue-num 0

El resultado debería ser el siguiente:

    iptables -L
    Chain INPUT (policy ACCEPT)
    ACCEPT     udp  --  anywhere             anywhere            udp dpt:654
    target     prot opt source               destination
    NFQUEUE    all  --  anywhere             anywhere            NFQUEUE num 0

    Chain FORWARD (policy ACCEPT)
    target     prot opt source               destination
    NFQUEUE    all  --  anywhere             anywhere            NFQUEUE num 0

    Chain OUTPUT (policy ACCEPT)
    target     prot opt source               destination
    ACCEPT     udp  --  anywhere             anywhere            udp dpt:654
    NFQUEUE    all  --  anywhere             anywhere            NFQUEUE num 0

Si luego queremos eliminar las rutas (para volverlas a añadir por ejemplo si
nos equivocamos al hacerlo al principio o si queremos dejar de usar meshias),
podemos ejecutar el comando iptables -F.

Hace falta también decirle al kernel que actúe como router, porque todos los
nodos de una red mesh puede servir de cartero para otros nodos:

    echo 1 > /proc/sys/net/ipv4/ip_forward

Finalmente ejecutamos meshias (también como superusuario, como los comandos
anteriores):

    meshias ath0

Cuya salida inicial debería ser:

    [1] Initializing
    [1] Daemon: opening aodv socket
    [1] Daemon initialized sucessfully
    local 192.168.0.1
    broadcast 192.168.0.255
    [1] Initilization done

Meshias se acapara del terminal donde se ejecuta para mostrar su salida. Para
probar su funcionamiento, lo que haremos un simple ping desde nuestra máquina
a la otra que debemos haber puesto con meshias ejecutando, y el ping lo haremos
desde otro terminal:

    ping 192.168.0.1 -c 5
    PING 192.168.0.1 (192.168.0.1) 56(84) bytes of data.
    64 bytes from 192.168.0.1: icmp_seq=1 ttl=64 time=55.161 ms
    64 bytes from 192.168.0.1: icmp_seq=2 ttl=64 time=48.213 ms
    64 bytes from 192.168.0.1: icmp_seq=3 ttl=64 time=47.137 ms
    64 bytes from 192.168.0.1: icmp_seq=4 ttl=64 time=49.168 ms
    64 bytes from 192.168.0.1: icmp_seq=5 ttl=64 time=48.175 ms

    --- 192.168.0.1 ping statistics ---
    5 packets transmitted, 5 received, 0% packet loss, time 3999ms
    rtt min/avg/max/mdev = 47.137/49.170/55.161/1.029 ms

La salida del meshias debería ser algo parecido a lo siguiente:
    
    [1] A packet was captured by nfqueue.
    STOLEN: Finding route for 192.168.0.1...
    route for 192.168.0.1 found, packet previously stolen accepted
    ACCEPT: packet for 192.168.0.1, route known
    ACCEPT: packet for 192.168.0.1, route known
    ACCEPT: packet for 192.168.0.1, route known
    ACCEPT: packet for 192.168.0.1, route known

¿Qué ha ocurrido bajo bmabalinas? Meshias ha capturado el paquete ICMP que manda
el comando ping. Ha visto que no tenemos ruta para ese destino, y ha enviado una 
petición de ruta AODV por broadcast a sus nodos vecinos. Como el nodo destino 
(192.168.0.1) es su vecino, éste le ha respondido directamente con otro paquete 
de respuesta de ruta AODV. El meshias que se ejecuta en nuestra máquina ha
recibido esta respuesta, ha añadido la nueva ruta a la tabla de rutas del kernel,
y finalmente ha dejado que se envíe el paquete ICMP que intentó el comando ping.

Como ya tenemos la ruta correcta, el ping se envía y recibe correctamente, y como
resultado el ping funciona. Podemos comprobar que se ha añadido una nueva ruta
a la tabla de rutas:

    route -n
    Kernel IP routing table
    Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
    192.168.0.1     192.168.0.1     255.255.255.255 U     0      0        0 ath0
    192.168.0.0     0.0.0.0         255.255.255.0   U     0      0        0 ath0

Si tuviésemos más nodos en nuestra red y el nodo destino no fuese un vecino nuestro
sino un nodo conectado por algún camino a través de otro nodo 192.168.0.3 por
ejemlo, el resultado final hubiese igualmente que el ping a 192.168.0.1 hubiese
funcionado, pero a la tabla de rutas de nuestra máquina hubiese una ruta algo
diferente: el gateway (puerta de enlace) para encontrar a 192.168.0.1 hubiese sido
192.168.0.3 en vez de sí mismo:

    route -n
    Kernel IP routing table
    Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
    192.168.0.1     192.168.0.3     255.255.255.255 U     0      0        0 ath0
    192.168.0.0     0.0.0.0         255.255.255.0   U     0      0        0 ath0

4. Cómo apagar meshias
----------------------

Con esto hemos demostrado básicamente cómo funciona meshias. Si ahora queremos
apagar el meshias, tenemos que pulsar Ctrl+C en el  terminal que está ejecutando
meshias para pararlo. Luego, eliminar las reglas de iptables. Si no tenemos
ninguna otra regla de iptables, podemos eliminar todas las reglas existentes
de iptables:

    iptables -F
    
O bien podemos eliminar las que antes añadimos una a una:
    
    iptables -t filter -D OUTPUT -o ath0 -p udp --dport 654 -j ACCEPT
    iptables -t filter -D INPUT -i ath0 -p udp --dport 654 -j ACCEPT
    iptables -t filter -D OUTPUT -o ath0 -j NFQUEUE --queue-num 0
    iptables -t filter -D INPUT -i ath0 -j NFQUEUE --queue-num 0
    iptables -t filter -D FORWARD -i ath0 -j NFQUEUE --queue-num 0
    
Si no queremos que nuestra máquina deje de enrutar paquetes, ejecutaríamos:

    echo 1 > /proc/sys/net/ipv4/ip_forward
    
Y para volver a poner la interfaz de red en modo Managed, pasa lo mismo que
antes, depende del driver de nuestra tarjeta de red esto se hace de distintas
maneras. Para tarjetas atheros:

    wlanconfig ath0 destroy
    wlanconfig ath0 create wlandev wifi0 wlanmode Managed
    
Para otras tarjetas, quizás nos sirva con hacer:
    
    iwconfig ath0 mode Managed
    
Finalmente, si estábamos usando Network Manager o algún otro demonio para
configurar la red wifi, es hora de volver a iniciarlo:

    /etc/init.d/NetworkManager start
    
5. Meshias: Comunidad y repercusión
-----------------------------------

Desde que comenzó el desarrollo de meshias a principio de curso, hemos trabajado
de forme intermitente en el proyecto. De hecho despues de un comienzo bastante
bueno, estuvimos un par de meses sin escribir una línea de código debido a
diversas razones, pero luego pusimos todas nuestras fuerzas en el desarrollo de
meshias y avanzamos a pasos agigantados. Tanto es así, que no dedicamos ni
tiempo para escribir en el blog.

No obstante ahora si estamos dedicando tiempo a eso, y estamos usando el blog
del proyecto para documentar lo que hemos hecho ya, y lo que estamos programando.
Cómo espero que hayáis podido comprobar con el tutorial anteriormente expuesto,
meshias ya es minimamente funcional, aunque aun faltan muchas cosas por hacer.

Hasta hace nada meshias no tenía desarrollado todos los componentes necesarios
para poder enrutar por lo que no era usable. A partir de ahora, ya funciona, y
nos encargaremos de crear una comunidad a su alrededor, de fomentar su uso y
asegurarnos su continuidad.
