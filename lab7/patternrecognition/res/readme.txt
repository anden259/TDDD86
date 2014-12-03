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
Is O(N^3) if you ignore the first values, 
since it's mostly constant factors playing a big role on small data

Sortering:
Is O(N^2) 




we used matlab to caluculate.

t=[400 800 1600 3200]
y=[1.2  9.128 73.265 593.734];
t1=[400 800 1600 3200 6400];
y1=[0.151 0.471 1.573 5.800 24.325 ];


(log(y(4))-log(y(3)))/(log(t(4))-log(t(3))) = 3.0186
(log(y1(5))-log(y1(4)))/(log(t1(5))-log(t1(4))) = 2.0683


