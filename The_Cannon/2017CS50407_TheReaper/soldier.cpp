#include "classes.h"
using namespace std;

soldier::soldier(int r, int c)
{
	this->r = r;
	this->c = c;	
}
int soldier::get_r()
{
	return r;
}
int soldier::get_c()
{
	return c;
}
void soldier::set_r_c(int r, int c)
{
	this->r = r;
	this->c = c;
}
