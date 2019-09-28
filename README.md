# Digital-Tachograph
Console application simulating digital tachograph installed in trucks in accordance with EU regulations on driving, working and break times
for truck drivers in EU.

This applications allows the user to simulate working conditions of digital tachograph. One can do this by choosing action or activity type
from the menu which makes changes to tachograph's status, such as information displayed, virtual time and date and data held within it.

When executed, the application uses system time as a base for virtual time of the tachograph which is later on changed according to lenght 
of particular activities done by the user.
The history of all activities is saved to two .txt files which represent the memory of both the tachograph as well as the digital 
drivercard which is inserted to the tachograph.
Tachograph warns the driver when he's reaching maximal lenght of the drive or shift time or if he's trying to begin new shift before 
minimal break time has been reached. It also counts the two-week-drive time which is also limited by regulations.

Some of the most crucial regulations in the EU are:

- the begining and the end of shift should be registered in the tachograph by selecting proper menu option which also requires determining 
country
- maximal drive time without break is 4.5 hours
- after that time 45 minutes break has to be made which can be divided into two parts from which first must be at least 15 minutes long and
second 30 minutes long (not the other way around!)
- maximal day drive time is 9 hours (10 hours two times a week)
- maximal shift time is 13 hours (15 hours three times a week)
- minimal break time is 11 hours (9 hours three times a week)
- maximal week drive time is 56 hours and two-weeks' drive time is 90 hours (on midnight from sunday to monday two-weeks drive time is 
being reset by subtracting the drive time of two weeks before) 

Not all of those regulations have been implemented yet though.
