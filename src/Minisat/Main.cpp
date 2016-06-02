#include <iostream>
#include <cmath>


#include "Solver.hpp"

using namespace std;

#define FOR(k,lb,ub) for (int k = (lb); (k) <= (ub); (k)++)


// 4 sommets de 1 à 4 et 2 couleurs de 1 à 2
int prop(int s, int c) // region et couleur
{ 
   
   return ((s-1)*4 + c); // on compte en base 6

}

bool voisines(int i, int j) 
{

   bool b ;
   b = (i==1 && j==2) || (i==2 && j==3) || (i==3 && j==4) || (i==4 && j==1);
   return b ;

}

int main() 
{
   Solver solver;

   vec<Lit> lits;

   // ajout des propositions
   // doit être = au nombre entier maximum d'une proposition + 1
   FOR(i,1,23)
   {
      solver.newVar();
   }


   // contraintes de voisinage
   FOR(i,1,4) 
   {
      FOR(j,1,4) 
      {
	    if (voisines(i,j))
	    {
	       FOR(c,1,2) 
	       {
		        solver.addBinary(~Lit(prop(i,c)),~Lit(prop(j,c)));
	       }
	    }
      }
   }


   // contrainte d'existence d'une couleur par pays
   FOR(i,1,4) 
   {
      lits.clear() ;
      FOR(c,1,2) 
      {
	    lits.push(Lit(prop(i,c))) ;
      }
      solver.addClause(lits);
   }
	 
   solver.solve();



   // récupération de la solution
   if (!solver.okay()) 
      {
	    cout << "Il n'y a pas de coloriage possible avec trois couleurs";
	  }
   else
   {
	 cout << "La formule est satisfaisable \n" ;
	 FOR(s,1,4) 
	 {
	    FOR(c,1,2) 
	    {
	       if (solver.model[prop(s,c)] == l_True) 
	       {
		      cout << " Le sommet " << s << " est colorie avec la couleur " << c ;
		      cout << "\n" ;
	       }
	    }
	 }
  }
}
