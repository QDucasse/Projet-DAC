# DAC Project

## Authors:
- K�vin BEDIN
- Quentin DUCASSE
- Steve Carlson KASSE MIKUI
- Guillaume LE BOUCHER
- Luc LONGIN

## Mise en place Github
1. Obtention du dossier sur votre compte et machine
Obtenir une copie du projet sur votre compte github : `fork`.
Pour cr�er une copie locale du projet git sur votre ordinateur :
```bash
$ git clone https://github.com/NomDeCompte/Projet-DAC
```

2. D�s que vous modifiez un �l�ment de votre dossier git, une `working copy` est cr��e. 
Visualisation du statut du dossier git :
```bash
$ git status
```
Ajout � la pile de `commit` 
```bash
$ git add nomDuFichier
```
Puis `commit` pour modifier les fichiers locaux :
```bash
$ git commit -m 'Message du commit'
```
La modification n'op�re que sur votre copie locale, ni la copie virtuelle GitHub de votre compte ni celle du compte d'o� provient le `fork` a �t� modifi�.

Pour modifier votre copie virtuelle :
```bash
$ git push
```
Pour proposer votre modification au dossier GitHub original, il faut effectuer une pull request, directement sur github via le bouton : `New pull request`

3. Maintenant, pour mettre � jour votre copie virtuelle et locale en cas de modification du dossier initial (celui que vous avez `fork`) il faut utiliser un `remote`. Le `remote` est un pointeur vers un le dossier git voulu :
Mise en place du `remote` appel� `upstream` par convention :
```bash
$ git remote add upstream https://github.com/bedinke/Projet-DAC
```
Mise � jour de votre dossier local et virtuel :
```bash
$ git fetch upstream
$ git checkout master
$ git merge upstream/master
```