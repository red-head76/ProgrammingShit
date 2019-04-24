def QuermultiplikationOhneListe(n):
    NeueZahl = 1
    counter = 1

    while n > 0:
        LetzteZiffer = n%10
        NeueZahl = NeueZahl*LetzteZiffer
        n = n//10
        if n == 0 and NeueZahl > 10:
            n = NeueZahl
            NeueZahl = 1
            counter +=1
    return(counter)

def QuermultiplikationMitListe(n):
    counter = 0    
    while (n > 9):
        NeueZahl = 1
        Ziffern = ZahlInListe(n)
        if not (Filter(Ziffern)) and counter == 0: # nur beim ersten mal wird gefiltert
            return -1
        for i in range(10):
            NeueZahl *= i**Ziffern[i]
        counter += 1
        n = NeueZahl
    return counter

def ZahlInListe(n):
    Ziffern = [0] * 10
    while (n > 0):
        LetzteZiffer = n%10
        n = n//10
        Ziffern[LetzteZiffer] += 1
    return Ziffern


def Filter(liste):
    if (liste[0] == 1 or liste[1] == 1 or liste[5] == 1):
        return False
    elif (liste[2] + liste[3] + liste[4] > 1):
        return False
    else:
        return True

def Zahlentest(nod, MitListe=True):
    counterMax = 1
    ZahlMax = 0
    for i in range(int(10**(nod-1)),int(10**(nod))+1):
        if MitListe:
            Iterations = QuermultiplikationMitListe(i)
        else:
            Iterations = QuermultiplikationOhneListe(i)
        if Iterations == -1:
            continue
        if Iterations > counterMax:
            counterMax = Iterations
            ZahlMax = i
    print("Best Number with %d digits: %d (%d Iterations)" % (nod, ZahlMax, counterMax))

import time

for i in range(10):
    start_time = time.time()
    Zahlentest(i)
    print("It took %f seconds" % (time.time() - start_time))
