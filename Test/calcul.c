// J'ai deja pensé a ça depuis un bout de temps mais comme le programme bug chez moi je vois pas trop comment l'implémenter. Si ça peut t'aider.

// Fonctions itermédiares

double degToRad(double deg){
	return deg * (3.14/180);
}

double latToY(double lat) {
	return 6378137 * log(tan(3.14/4 + degToRad(lat)/2));
}

double lonToX(double lon) {
	return degToRad(lon) * 6378137;
}

// Pour le ratio je vois ça comme ça

// Ca donne le ratio a utilisé pour la fenetre
double deltaX = lonToX(maxlon) - lonToX(minlon);
double deltaY = latToY(maxlat) - latToY(minlat);
double ratio = deltaX / deltaY;

// En fonction de la map tu peux choisir la taille de la fenetre.
window_Height = MAX_HEIGHT;
		window_Width = ratio * MAX_HEIGHT;
		if (window_Width > MAX_WIDTH){
			window_Width = MAX_WIDTH;
			window_Height = window_Width / ratio;
		}

// T'as aussi l'échelle comme ça.
double scale = window_Height / deltaY;
