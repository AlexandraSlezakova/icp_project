# The C++ Programming Language Project

## Application for displaying public transport lines and simulating their movement
Authors: Alexandra Slezakova, Martin Vadura

## How to run
`make run` - compile and run application
`make doxygen` - generate doxygen documentation into `doc`

## How to use
Streets, bus stops and buses are loaded from file. Timer is set to current time and can be changed by button `Change time`.
After this action start of timer is needed. Reset timer changes time to system time and sets timer interval to default value.
Changing time interval is also possible by `plus` and `minus` buttons.

Each bus moves along its route. Hovering over the bus shows its number, clicking shows route and bus timetable
with route number and bus id.

Zoom in and out whole map is possible by `plus` and `minus` buttons under the text area and mouse wheel.

### Slowdown
List of streets is in combo box. Right after street selection, use slider located next to the combo box
to change slowdown on selected street. Yellow color The yellow color indicates the medium-fast slowdown,
red color indicates high slowdown.

### Roadblock
For roadblock change RoadBlockMode to ON by clicking on button. Selection of new route for bus is needed and can be done 
by clicking on squares.
When the button changes color to green, you can choose between two options:
    - close the street between 2 stops
    - close the bus stop
color of squares is red.
After clicking on the bus, already traveled route turns pink and new clicked route has same color. When the new route ends 
and bus can continue on the normal route, it is necessary to confirm whether the bus should continue. If option "No" is selected,
bus will be deleted at the end of its route.
The new route is saved after clicking on any bus or turning RoadBlockMode OFF. 
