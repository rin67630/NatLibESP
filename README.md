# NatLibESP
Not a Time LIB for ESP devices, but...
This is NOT another WiFi Time library for the ESP world, adding confusion to the jungle of incompatible libraries!

I really got sick about the high amount of libraries apparently doing (more or less well) the same, jeopardizing each other, being only valid under special conditions and frequently limited to provide a single printed output.

I decided to reconsider the problem and provide only an extensive code example that can be used by everybody to build on their own code. The solution must be fully portable and run on Espressifs ESP32 and ESP8266 based boards without modification. The solution must:

-provide the Posix methods to deal with dates and strings and compute time differences
-provide examples for serial, but also examples for working with text buffers as being used for displays or communication protocols.
-provide the usual variables Second, Minute, Hour, Day, Month, Year as integers, DayName and MonthName as char arrays.
-be able to run a certain time off-line and periodically synchronize with NTP.
-provide the following subroutines: void getWiFi() void getNTP() void getEpoch() void getTimeData()

The code uses only the libs delivered on-board from Espressif.

Sometimes you may be invited to use the Arduino TimeLib Library currently maintained by Paul Stoffregen... Please do not do that! This code provides the main outputs in the same way. I have chosen to only use the common part of Espressif's library based on Posix functionality to get the different parts into the variables and the preconfigured strings.

Finally I decided to provide some printed outputs to demonstrate the usage of the different outputs and methods into this looped message using different methods to prepare that didactic message:

<i>Thanks God, it is: Thursday! Date is 07/05/2020 and Time is 23:07:25 Now is 23 Hour, 07 Minutes and 25 Seconds. The Epoch is: 1588892845 Date is Thursday, 07 May 2020 Americans say it is 11:07PM, but Europeans prefer using 23:07:25</i>

Of course you will repace that lines with your own art... Enjoy!
