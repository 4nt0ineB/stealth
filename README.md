# stealth

# Coding standards

### Language
English for both code and documentation


### Function name 
Choose one !

(snake_case)<br>
<span>+ More readable</span><br> 
<span>- quite long name</span>
```c 
void guard_attack_player()
```

or

(camelCase)<br>
<span>- Less readable</span><br>
<span>- Shorter</span>
```c
void guardAttackPlayer()
```

### Error codes 
It's good to define error codes with enum.
Obvious functions can return the proper data, 
otherwise we have to return error codes and handle return pointer in parameters. 
As stated in course.  


### Important
"Le flag -lMLV est normalement réservé aux modules graphiques
ainsi qu’à l’assemblage de l’exécutable (sinon, c’est que votre interface graphique dé-
gouline de partout et que vos sources sont mal modulées)."


### Credits

wall sprites from [Michele Bucelli](https://opengameart.org/users/buch)