/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include <iostream>
#include "String.h"

#include "ObjectConsumer.h"
#include "ObjectWriter.h"

class Object;


ObjectConsumer::ObjectConsumer(String const & name,
                               ObjectWriter * writer)
    : m_name(name),
      m_writer(writer),
      m_count(0)
{
  if (m_writer != 0)
    m_writer->open();
}

ObjectConsumer::~ObjectConsumer()
{
  if (m_writer != 0)
    delete m_writer;
  m_writer = 0;
}

void ObjectConsumer::finished()
{
  if (m_writer != 0)
    m_writer->close();
  std::cout << getName() << " processed " << m_count << " objects." << std::endl;
}
