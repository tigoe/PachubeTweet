Tweeting from Arduino though Pachube

This set of sketches allows you to send tweets to twitter.com by using Pachube.com as a middleware.  Pachube receives the tweet from the Arduino via a HTTP PUT requests, extracts the data, and passes it on to Twitter.

The basic structure is the same for the Arduino Ethernet and WiFi shields. Examples for both are included here.

To make this work you should be familiar with the basics of the Ethernet or WiFi shield, with Pachube.com, and with Twitter.com.  You might also benefit from knowing about HTTP requests and JSON.

To use either of these examples, simply download them and run them on an Arduino with the appropriate shield attached. They use libraries that are included with the Arduino distribution.

More info on these sketches can be found at http://www.tigoe.com/pcomp/code/?p=1135

-Tom Igoe
