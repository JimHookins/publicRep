#include "rootcontext.h"

RootContext::RootContext()
{

}

QSqlTableModel * RootContext::getModel()
{
    return db.getModel();
}

void RootContext::updateModel()
{
    db.updateModel();
}
