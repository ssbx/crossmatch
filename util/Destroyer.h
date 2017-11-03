/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#ifndef DESTROYER_DEFINED
#define DESTROYER_DEFINED

/*
 * Destroyes a singleton.      
 *
 * Taken from:
 *
 * http://www.sigs.com/publications/docs/cppr/9606/cppr9606.c.vlissides.html
 *
 * which was published in C++ Report sometime ...
 *
 * Note: doesn't work with the g++ compiler prior to version 2.8 due 
 * to its lack of support for templated friend classes.
 */


template <class DOOMED>
class Destroyer
{
  public:
    Destroyer(DOOMED * d = 0);

    ~Destroyer();

    void setDoomed(DOOMED * d);

  private:
    DOOMED * m_doomed;

    // Prevent users from making copies of a
    // Destroyer to avoid double deletion:
    Destroyer(Destroyer<DOOMED> const &);

    Destroyer<DOOMED> & operator=(Destroyer<DOOMED> const &);
};


template <class DOOMED>
Destroyer<DOOMED>::Destroyer(DOOMED * d)
    : m_doomed(d)
{
}

template <class DOOMED>
Destroyer<DOOMED>::~Destroyer()
{
    delete m_doomed;
}

template <class DOOMED>
void Destroyer<DOOMED>::setDoomed(DOOMED * d)
{
    m_doomed = d;
}

template <class DOOMED>
Destroyer<DOOMED>::Destroyer(Destroyer<DOOMED> const &)
{
}

template <class DOOMED>
Destroyer<DOOMED> &
Destroyer<DOOMED>::operator=(Destroyer<DOOMED> const &)
{
    return *this;
}


#define DestroyerFriend(x)       friend class Destroyer<x>;
#define DestroyerDefine(x)       static Destroyer<x> s_destroyer;
#define DestroyerDeclare(x)      Destroyer<x> x::s_destroyer;
#define DestroyerInit(x)         s_destroyer.setDoomed(x);


#endif  // DESTROYER_DEFINED
