Here is a nice tutorial: http://nthn.me/posts/2012/mqtt.html
# Introduction #

[Mosquitto](http://mosquitto.org/) is an Open Source MQTT v3.1 message broker written by Roger Light.

# Installing Mosquitto #

Mosquitto can be installed on recent versions of Ubuntu from the normal package repository: [mosquitto](http://manpages.ubuntu.com/cgi-bin/search.py?q=mosquitto), [mosquitto\_pub](http://manpages.ubuntu.com/cgi-bin/search.py?q=mosquitto_pub) and [mosquitto\_sub](http://manpages.ubuntu.com/cgi-bin/search.py?q=mosquitto_sub) are in the official repositories since the [Oneiric Ocelot](https://wiki.ubuntu.com/OneiricOcelot) (Ubuntu 11.10) release.

<pre>
$ sudo apt-get install mosquitto<br>
$ sudo apt-get install mosquitto-clients<br>
</pre>

# Subscribing to a Topic #

<pre>
$ mosquitto_sub -d -t hello/world<br>
</pre>

# Publishing to a Topic #

<pre>
$ mosquitto_pub -d -t hello/world -m "Hello World"<br>
</pre>