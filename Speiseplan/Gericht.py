import random
#random.seed(a=285722435682023485720103510874561230541235610873561028756192834710289347)

i = 0
speiseplan = open("Speiseplan.txt", "r").readlines()
bereits_angeboten = []

ja_liste = open("ja.txt", "r").readlines()
nein_liste = open("nein.txt", "r").readlines()
ja = []
nein = []

for j in ja_liste:
    ja.append(j[:-1])
    
for k in nein_liste:
    nein.append(k[:-1])
    
def kein_bock():
    global i
    if i == 0:
        string = 'Wie wärs mit'
    elif i == 1:
        string = 'Oder vielleicht'
    elif i == 2:
        string = 'Doch lieber'
    elif i == len(speiseplan):
        print('Dann musste halt hungern!')
        exit()
    else:
        string = 'Dann halt'
    
    neuer_vorschlag = True
    while neuer_vorschlag:
        gericht = speiseplan[random.randint(0, len(speiseplan)-1)]
        if gericht not in bereits_angeboten:
            neuer_vorschlag = False
    bereits_angeboten.append(gericht)
    print(string, gericht[:-1])
    i += 1

if __name__ == "__main__":
    kein_bock()
    while (True):
        inp = input()
        if (inp in nein):
            kein_bock()
            continue
        elif (inp in ja):
            print("Guten Hunger!")
            break
        else:
            continue
