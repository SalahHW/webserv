En theorie les connections sont gere, de tel sorte qu'on a un socket de connexion qui attend, qui ecoute.
Une fois quil entend quelque chose le programme creer un nouveau socket pour la connexion specifique ainsi le premier socket de connexion reste disponible.
Maintenant il faut parser cette requette afin de pouvoir y repondre.
Il faut le faire de la meme maniere que nginx.
C'est a dire faire une machine a etat. Demande a Chatgpt comment parcer une requete http de la meme maniere que nginx.
