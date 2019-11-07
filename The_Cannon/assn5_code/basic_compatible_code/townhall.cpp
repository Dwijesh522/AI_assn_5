#include "classes.h"
using namespace std;

townhall::townhall(int r, int c)
{
	this->r = r;
	this->c = c;
}
int townhall::get_r()
{
	return r;
}
int townhall::get_c()
{
	return c;
}
