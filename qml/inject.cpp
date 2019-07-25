#include "inject.h"

QPointer<Inject>  Inject::o=nullptr;
Inject::Inject(QObject *parent) : QObject(parent){

}
