#Projet Encodeur JPEG

Pour lancer le programme avec echantillonage : ./ppm2jpeg image --sample 2x2,1x1,1x1 (par exemple) 
Pour choisir le nom de l'image : ./ppm2jpeg image --output-file nom

**Script Shell qui teste chaque échantillonage sur une image, créé un dossier contenant toutes ces images:**
**exemple** ./test.sh images/shaun-the-sheep.ppm shaun

##Rôle des différents modules :

###readPPM : 
* **éléments requis** : chemin vers l'image ppm, nom choisi par l'utilisateur (si un nom est choisi)
* **sortie** : retourne une structure imagePPM, qui comporte une matrice de pixels couleur ou noir et blanc, le nom de l'image, le chemin, la hauteur et la largeur
* **rôle** : lit le fichier ppm, et remplit une structure imagePPM

###imageToMCUs : 
* **éléments requis** : une structure ImagePPM avec tous ses éléments 
* **sortie** : une structure MCUsMatrice, qui contient un tableau des différents MCUs, le nombre de lignes et le nombre de colonnes (pour s'en servir plus tard comme une matrice)
* **rôle** : analyse les pixels de l'image, les sépare en MCUs et retourne une structure avec ces MCUs, dans le cas où la hauteur ou la largeur n'était pas un multiple de 8, duplique la dernière colonne/ligne avant de séparer en MCUs. Libère la mémoire utilisée par les pixels contenus dans la structure ImagePPM.

###operationMCU
* **éléments requis** : une structure MCUsMatrice 
* **sortie** : une structure MCUsTransformMat, qui contient un tableau des MCUs après la transformation RGB à YCbCr, et les étapes jusqu'à quantification
* **rôle** : transforme une MCUSMatrice qui contient les MCUs qui ont les pixels à une MCUsTransformMat qui contient les MCUs avec les valeurs de Y, Cb, Cr après les étapes jusqu'à la quantification. La module se sert du module algo_matrices.

###algo_matrices
* **éléments requis** : une matrice 
* **sortie** : la matrice après zigzag ou après DCT ou quantification 
* **rôle** : transforme une matrice en lui appliquant les algorithmes cités, le DCT a été optimisé 

###codage_huffman_RLE
* **éléments requis** : une structure MCUsTransformMat
* **sortie** : le fichier jpeg 
* **rôle** : 

###echantillonage
* Fusionne et échantillone les MCUS.

###structures
* Répertorie les différentes structures utilisées, qui sont utilisées à plusieurs endroits différents

###affichageStructures 
* Contient les fonctions pour afficher n'importe quelle structure. 

###libererStructures 
* Contient les fonctions pour libérer n'importe quelle structure. 

###test_malloc
* Contient une fonction qui teste si un pointeur est NULL, utilisé à chaque fois que nous faisons un malloc/calloc.