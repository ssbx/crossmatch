/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include <iostream>
#include "String.h"

#include "ObjectPairConsumer.h"
#include "ObjectPairWriter.h"

class Object;


ObjectPairConsumer::ObjectPairConsumer(String const & name,
                                       ObjectPairWriter * writer)
    : m_writer(writer),
      m_count(0),
      m_name(name)
{
  if (m_writer != 0)
    m_writer->open();
}

ObjectPairConsumer::~ObjectPairConsumer()
{
  if (m_writer != 0)
    delete m_writer;
  m_writer = 0;
}

void ObjectPairConsumer::finished()
{
  if (m_writer != 0)
    m_writer->close();
  std::cout << getName() << " processed " << m_count << " object pairs." << std::endl;
}
