#include <stdio.h>
#include "Poly.h"
#include<iostream>
#include<string>
#include<sstream>
#include<cmath>
using namespace std;
/*
* 
* @author Kubra Korkmaz
* @date 31.07.2022
* 
*/
//-------------------------------------------------
// Creates a polynomial from an expression.
// Returns a pointer to the first PolyNode in the list (head of the list)
//
// Expression will be of the following sort:
// Ex1: 2.3x^4 + 5x^3 - 2.64x - 4
// Ex2: -4.555x^10 - 45.44
// Ex3: x^6 + 2x^4 - x^3 - 6.3x + 4.223 
// Ex4: 34
// Ex5: -x+1
// Ex6: -3x^4    +   4x
// Ex7: -2x  - 5
//

void parsingTerm(string term, string &exp, string &cff) {
	string s = " "; 
	if (term.find('x') != string::npos && term.find('^') != string::npos) {//if term has coeff>1 and exp>1
		for (int i = 0; i < term.size(); i++) {
			s += term[i];//collect char
			if (term[i] == 'x') {//collect all char until encounter x char
				s.pop_back();
				if (s[i] == '-') {//if initial term is -x
					//clear string
					s.clear();
					//s is -1.0
					s = "-1.0";
				}
				else if (s[i] == ' ') {//if initial term is x
					//s is 1.0
					s = "1.0";
				}
				//set s to coef of term
				cff = s;
				//clear string
				s.clear();
			}
			if (term[i] == '^') {//if encounter exp char pop back it
				s.pop_back();
			}
		}
		exp = s;//set s string to exp
		s.clear(); //clear string		
	}
	else if (term.find('x') != string::npos) {//if term has coeff>1 and exp=1		
		for (int i = 0; i < term.size(); i++) {
			s += term[i]; //collect char

			if (term[i] == 'x') {//if char is encounter x character
				s.pop_back();//pop back x character

				if (s == "-") {//if initial term is -x
					s.pop_back();//pop back - character
					s = "-1.0";  //set -1.0 to s
				}
				else if (s == " ") {//if initla term is x
					s = "1.0"; //set 1.0 to s
				}

				cff = s; //set s to cff
				exp = '1'; //set 1 to exp of term
				s.clear(); //clear string
			}
		}		
	}
	else {// if term has only coef such that 22, -1 , 0	

		if (term.find(' ') == string::npos) {//skip '/0' character 

			for (int i = 0; i < term.size(); i++) {
				s += term[i];//collect all characters
			}

			exp = '0';//set 0 to exp
			cff = s;  //set s to cff
			s.clear(); //clear string
		}	
	}
}
PolyNode *CreatePoly(char *expr){
	PolyNode* node = NULL;
	string s = " ";
	string ss = " ";
	char n = ' ';
	int i = 0;	

	while (expr[i]) {//collect all character into string except for wspace character
		if (expr[i] != ' ') { s += expr[i]; }
		i++;
	}
	//get size of string which has any wspace character
	size_t size = s.size();
	//allocate one dimensional dynamic array
	string* arr = new string[size];
	size_t counter = 0;
	for (size_t i = 0; i < s.size(); i++) {		
		if (s[i] == '+') {//check next term is positive 
			i++; //skip + character
			arr[counter] = ss;//insert pre terms into dynamic array
			counter++; //increase counter
			ss.clear();//clear string
		}
		else if (s[i] == '-') {//check next term is negative 
			arr[counter] = ss; //insert pre term into dynamic array 
			counter++; //increase counter
			ss.clear(); //clear string
		}
		ss += s[i]; //collect characters	
	}
	arr[counter] = ss; //insert last term into dynamic array

	float cff = 0;
	int exp = 0;
	string a = ""; //exp of term
	string b = ""; //coef of term
	stringstream strm;

	for (i = 0; i <= counter; i++) {
		//invoke funciton
		parsingTerm(arr[i], a, b); //get coef and exp value of term 
		//insert b into strm varible and convert string b variable into float cff and clear strm 
		strm << b; strm >> cff; strm.clear();
		//insert a into strm varible and convert string a variable into int exp and clear strm 
		strm << a; strm >> exp; strm.clear();
		//create poly
		node = AddNode(node, cff, exp);
	}
	//delete dynamic array
	delete[]arr;
	arr = NULL; //set null
	return node;
} //end-CreatePoly

/// -------------------------------------------------
/// Walk over the poly nodes & delete them
///
void DeletePoly(PolyNode* poly) {
	//delete all poly
	PolyNode* node; 
	node = poly;
	while (poly != NULL) {//delete all poly terms
		poly = poly->next;
		delete node;
		node = poly;
	}//end-while	
} // end-DeletePoly
	
//-------------------------------------------------
// Adds a node (coefficient, exponent) to poly. If there already 
// is a node with the same exponent, then you simply add the coefficient
// to the existing coefficient. If not, you add a new node to polynomial
// Returns a pointer to the possibly new head of the polynomial.
//
PolyNode* AddNode(PolyNode *head, double coef, int exp){
	//create a new node and allocate with coef and exp values
	PolyNode* p = new PolyNode();
	p->coef = coef;
	p->exp = exp;
	//terminate node
	p->next = NULL;
	//find the location where we insert new value
	PolyNode* q = NULL;
	//if head null then insert p to head (initial value)
	if (head == NULL) {
		head = p;
	}//end -if
	else if (p->exp == head->exp) {//if head and next values exp equal then sum 
		head->coef += coef;
		if (head->coef == 0) {//if coef of head is 0 then move head to next value
			p = head->next;
			head = p;
		}//end-if
	}//end-else-if
	else if (p->exp > head->exp) {	//if node exp greater head exp then 
		//head is node and next of head is previous head.									
		p->next = head;
		head = p;
	}//end-else-if
	else {//head is greater than new node
		q = head;
		while (q->next != NULL && q->next->exp >= exp) {
			//exp of next node of q is equal to exp then sum 
			if (q->next->exp == exp) {
				q->next->coef += coef;
				//if coef of next node of q is 0 then p equal to its next <3x^2 + 0.0 + 1>
				//next node of q is p then return. <3x^2 + 1>
				if (q->next->coef == 0) {					
					p = q->next->next;
					q->next = p;
					return head;
				}//end-if
			}//end-if
			//move q to next q node
			q = q->next;			
		}//end-while
		if (q->exp != exp) { //if exp of q not equal to exp and greater than it then insert new value after q
			p->next = q->next;
			q->next = p;
		}//end-if		
	}//end-else
	return head;
} // end-AddNode

//-------------------------------------------------
// Adds two polynomials and returns a new polynomial that contains the result
// Computes: poly3 = poly1 + poly2 and returns poly3
//
PolyNode *Add(PolyNode *poly1, PolyNode *poly2){
	PolyNode* poly3 = new PolyNode();
	while (poly1 != NULL && poly2 != NULL) {		
		if (poly1->exp == poly2->exp) { //if two poly expressions have same exp
			//sum coefs and set one of them exp
			poly3 = AddNode(poly3, poly1->coef + poly2->coef, poly1->exp); //create new poly term
			poly2 = poly2->next; //move next term
			poly1 = poly1->next; //move next term
		}
		else if (poly1->exp > poly2->exp) { 
			poly3 = AddNode(poly3, poly1->coef, poly1->exp);
			poly1 = poly1->next;
		}
		else{
			poly3 = AddNode(poly3, poly2->coef, poly2->exp);
			poly2 = poly2->next;
		}	
	}
	return poly3;
} //end-Add

//-------------------------------------------------
// Subtracts poly2 from poly1 and returns the resulting polynomial
// Computes: poly3 = poly1 - poly2 and returns poly3
//
PolyNode *Subtract(PolyNode *poly1, PolyNode *poly2){
	PolyNode* poly3 = new PolyNode();
	while (poly1 != NULL && poly2 != NULL) {
		if (poly1->exp == poly2->exp) {//if two poly expressions have same exp
			//substrac poly2 coef from poly1 coef and insert exp of one of them 
			poly3 = AddNode(poly3, poly1->coef - poly2->coef, poly1->exp); //create new poly term
			poly2 = poly2->next;//move next term
			poly1 = poly1->next; //move next term
		}
		else if (poly1->exp > poly2->exp) {
			poly3 = AddNode(poly3, poly1->coef, poly1->exp);
			poly1 = poly1->next;
		}
		else {
			poly3 = AddNode(poly3, (-1)*poly2->coef, poly2->exp);
			poly2 = poly2->next;
		}
	}
	return poly3;
} //end-Substract

//-------------------------------------------------
// Multiplies poly1 and poly2 and returns the resulting polynomial
// Computes: poly3 = poly1 * poly2 and returns poly3
//
PolyNode *Multiply(PolyNode *poly1, PolyNode *poly2){
	PolyNode* poly3 = new PolyNode();
	PolyNode* q = NULL;
	while (poly1 != NULL) {
		q = poly2;//reuse poly2
		while (q != NULL) {
			poly3 = AddNode(poly3, poly1->coef * q->coef, poly1->exp + q->exp);			
			q = q->next; //move next term
		}
		poly1 = poly1->next; //move next term
	}
	return poly3;
} //end-Multiply

//-------------------------------------------------
// Evaluates the polynomial at a particular "x" value and returns the result
//
double Evaluate(PolyNode *poly, double x){
	int e = 0;
	double c = 0;
	double result = 0;
	while (poly != NULL)
	{
		e = poly->exp;
		c = poly->coef;
		result += pow(x, e) * c;
		poly = poly->next;
	}
	return result;
} //end-Evaluate

//-------------------------------------------------
// Computes the derivative of the polynomial and returns it
// Ex: poly(x) = 3x^4 - 2x + 1-->Derivative(poly) = 12x^3 - 2
//
PolyNode *Derivative(PolyNode *poly){
	PolyNode* p = new PolyNode();
	while (poly != NULL) {
		
		if (poly->exp == 0) {//derivative of a constant
			p = AddNode(p, 0, 0);
		}
		else {
			poly->coef *= poly->exp;
			poly->exp -= 1;
			p = AddNode(p, poly->coef, poly->exp); //create new poly term
		}		
		poly = poly->next;
	}
	return p;
} //end-Derivative

//-------------------------------------------------
// Plots the polynomial in the range [x1, x2].
// -39<=x1<x2<=39
// -12<=y<=12
// On the middle of the screen you gotta have x & y axis plotted
// During evaluation, if "y" value does not fit on the screen,
// then just skip it. Otherwise put a '*' char depicting the curve
//
void Plot(PolyNode *poly, int x1, int x2){
	int r = 0; //result
	int i = -39;
	for (int j = 12; j >= -12; j--) {
		for (int i = x1; i <= x2; i++) {
			r = Evaluate(poly, i); 
			if (j == r) { cout << "*"; }
			else if (i == 0 && j != 0) { cout << "|"; }
			else if (i == 0 && j == 0) { cout << "|"; }
			else if (j == 0 && i != 0) { cout << "-"; }
			else if (j != 0 && i != 0) { cout << " "; }
		}
		cout << endl;
	}	
} //end-Plot
