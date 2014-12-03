/**********************************************************************
 *  Mönsterigenkänning readme.txt
 **********************************************************************/

/**********************************************************************
 *  Empirisk    Fyll i tabellen nedan med riktiga körtider i sekunder
 *  analys      när det känns vettigt att vänta på hela beräkningen.
 *              Ge uppskattningar av körtiden i övriga fall.
 *
 **********************************************************************/
    
      N       brute       sortering
 ----------------------------------
    150		0,059			0,057
    200		0,197			0,073
    300		0,540			0,105
    400		1,200			0,151
    800		9,128			0,471
   1600		73,265			1,573
   3200		593,734			5,800
   6400						24,325
  12800


/**********************************************************************
 *  Teoretisk   Ge ordo-uttryck för värstafallstiden för programmen som
 *  analys      en funktion av N. Ge en kort motivering.
 *
 **********************************************************************/

Brute:
Is O(N^4) (average N^3)
It's a loop in a loop in a loop in a loop, each loop is N or less.

Sortering:
Is O(N^2)
Stepping through each point once is N
and then check the other points from their is N or less.

map operator[] is logarithmic
map insert is also logarithmic
looping through our map and checking vector size is less then N
rendering is constant

conclusion N^2


--------

we used matlab to caluculate.

t=[400 800 1600 3200]
y=[1.2  9.128 73.265 593.734];
t1=[400 800 1600 3200 6400];
y1=[0.151 0.471 1.573 5.800 24.325 ];


(log(y(4))-log(y(3)))/(log(t(4))-log(t(3))) = 3.0186
(log(y1(5))-log(y1(4)))/(log(t1(5))-log(t1(4))) = 2.0683


