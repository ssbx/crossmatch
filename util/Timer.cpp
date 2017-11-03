/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "String.h"
#include <iostream>
#include <iomanip>

#include "Timer.h"
#include "Destroyer.h"


#ifdef TIMER
Timer * Timer::s_instance = 0;
DestroyerDeclare(Timer)

void Timer::reportTimes()
{
  Timer * t = globalTimer();
  t->pause();
  std::cout << *t;
  t->cont();
}
#endif

Timer::Timer(String const & name, State initialState)
    : state(initialState),
      time_now(),
      resource_usage(),
      label(),
      time_start(0.0),
      cpu_start(0.0),
      user_cpu_start(0.0),
      system_cpu_start(0.0),
      max_memory_start(0),
      shared_memory_start(0),
      unshared_data_memory_start(0), 
      unshared_stack_memory_start(0),
      page_faults_start(0),
      major_page_faults_start(0),
      minor_page_faults_start(0),
      blocked_input_ops_start(0),
      blocked_output_ops_start(0),
      time_accum(0.0),
      cpu_accum(0.0),
      user_cpu_accum(0.0),
      system_cpu_accum(0.0),
      max_memory_accum(0),
      shared_memory_accum(0),
      unshared_data_memory_accum(0),
      unshared_stack_memory_accum(0),
      page_faults_accum(0),
      minor_page_faults_accum(0),
      major_page_faults_accum(0),
      blocked_input_ops_accum(0),
      blocked_output_ops_accum(0)
{
  reset(name, initialState);
}

Timer::~Timer()
{
}

String const & Timer::name() const
{
  return label;
}

void Timer::reset(String const & name, State newState)
{
  label = name;
  state = newState;
  if (newState != Paused)
    start();
  time_accum = cpu_accum = user_cpu_accum = system_cpu_accum = 0.0;
  max_memory_accum = shared_memory_accum = 0;
  unshared_data_memory_accum = unshared_stack_memory_accum = 0;
  page_faults_accum = minor_page_faults_accum = major_page_faults_accum = 0;
  blocked_input_ops_accum = blocked_output_ops_accum = 0;
}

double Timer::elapsed(bool timeCalled)
{
  double t = time_accum;
  if (state == Running)
  {
    if (!timeCalled)
      updateTime();
    t += currentElapsed() - time_start;
  }

  return t;
}

double Timer::cpu(bool usageCalled)
{
  double t = cpu_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentCpu() - cpu_start;
  }

  return t;
}

double Timer::user_cpu(bool usageCalled)
{
  double t = user_cpu_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentUserCpu() - user_cpu_start;
  }

  return t;
}

double Timer::system_cpu(bool usageCalled)
{
  double t = system_cpu_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentSystemCpu() - system_cpu_start;
  }

  return t;
}

long int Timer::max_memory(bool usageCalled)
{
  long t = max_memory_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentMaxMemory() - max_memory_start;
  }

  return t;
}

long int Timer::shared_memory(bool usageCalled)
{
  long t = shared_memory_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentSharedMemory() - shared_memory_start;
  }

  return t;
}

long int Timer::unshared_data_memory(bool usageCalled)
{
  long t = unshared_data_memory_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentUnsharedDataMemory() - unshared_data_memory_start;
  }

  return t;
}

long int Timer::unshared_stack_memory(bool usageCalled)
{
  long t = unshared_stack_memory_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentUnsharedStackMemory() - unshared_stack_memory_start;
  }

  return t;
}

long int Timer::page_faults(bool usageCalled)
{
  long t = page_faults_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentPageFaults() - page_faults_start;
  }

  return t;
}

long int Timer::minor_page_faults(bool usageCalled)
{
  long t = minor_page_faults_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentMinorPageFaults() - minor_page_faults_start;
  }

  return t;
}

long int Timer::major_page_faults(bool usageCalled)
{
  long t = major_page_faults_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentMajorPageFaults() - major_page_faults_start;
  }

  return t;
}

long int Timer::blocked_input_ops(bool usageCalled)
{
  long t = blocked_input_ops_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentBlockedInputOps() - blocked_input_ops_start;
  }

  return t;
}

long int Timer::blocked_output_ops(bool usageCalled)
{
  long t = blocked_output_ops_accum;
  if (state == Running)
  {
    if (!usageCalled)
      updateResourceUsage();
    t += currentBlockedOutputOps() - blocked_output_ops_start;
  }

  return t;
}

void Timer::pause()
{
  if (state == Running)
  {
    state = Paused;
    updateTime();
    time_accum += currentElapsed() - time_start;
    updateResourceUsage();
    cpu_accum += currentCpu() - cpu_start;
    user_cpu_accum += currentUserCpu() - user_cpu_start;
    system_cpu_accum += currentSystemCpu() - system_cpu_start;
    max_memory_accum += currentMaxMemory() - max_memory_start;
    shared_memory_accum += currentSharedMemory() - shared_memory_start;
    unshared_data_memory_accum += currentUnsharedDataMemory() - unshared_data_memory_start;
    unshared_stack_memory_accum += currentUnsharedStackMemory() - unshared_stack_memory_start;
    page_faults_accum += currentPageFaults() - page_faults_start;
    minor_page_faults_accum += currentMinorPageFaults() - minor_page_faults_start;
    major_page_faults_accum += currentMajorPageFaults() - major_page_faults_start;
    blocked_input_ops_accum += currentBlockedInputOps() - blocked_input_ops_start;
    blocked_output_ops_accum += currentBlockedOutputOps() - blocked_output_ops_start;
  }
}

void Timer::cont()
{
  if (state == Paused)
  {
    state = Running;
    start();
  }
}

void Timer::getTimes(double & elapsed, double & user_cpu,
                     double & system_cpu, double & cpu,
                     long & max_memory, long & shared_memory,
                     long & unshared_data_memory, long & unshared_stack_memory,
                     long & page_faults, long & minor_page_faults,
                     long & major_page_faults, long & blocked_input_ops,
                     long & blocked_output_ops)
{
  updateTime();
  elapsed = this->elapsed();
  updateResourceUsage();
  cpu = this->cpu(true);
  user_cpu = this->user_cpu(true);
  system_cpu = this->system_cpu(true);
  max_memory = this->max_memory(true);
  shared_memory = this->shared_memory(true);
  unshared_data_memory = this->unshared_data_memory(true);
  unshared_stack_memory = this->unshared_stack_memory(true);
  page_faults = this->page_faults(true);
  minor_page_faults = this->minor_page_faults(true);
  major_page_faults = this->major_page_faults(true);
  blocked_input_ops = this->blocked_input_ops(true);
  blocked_output_ops = this->blocked_output_ops(true);
}

std::ostream & operator<<(std::ostream & os, Timer & timer)
{
  double elapsed, user_cpu, system_cpu, cpu;
  long max_memory, shared_memory, unshared_data_memory, unshared_stack_memory;
  long page_faults, minor_page_faults, major_page_faults;
  long blocked_input_ops, blocked_output_ops;
  timer.getTimes(elapsed, user_cpu, system_cpu, cpu,
                 max_memory, shared_memory, unshared_data_memory, unshared_stack_memory,
                 page_faults, minor_page_faults, major_page_faults,
                 blocked_input_ops, blocked_output_ops);

  int prec = os.precision();
  os << "Timer " << timer.name() << ":" << std::endl;
  os << std::setprecision(8);
  os << "\treal:   " << elapsed    << "\t\t" << elapsed/60.0    << std::endl;
  os << "\tuser:   " << user_cpu   << "\t\t" << user_cpu/60.0   << std::endl;
  os << "\tsystem: " << system_cpu << "\t\t" << system_cpu/60.0 << std::endl;
  os << "\tcpu:    " << cpu        << "\t\t" << cpu/60.0        << std::endl;

  return os << std::setprecision(prec);
/*
  os << " max mem= " << max_memory << "\t";
  os << " shared mem= " << shared_memory << "\t";
  os << " data mem= " << unshared_data_memory << "\t";
  os << " stack mem= " << unshared_stack_memory << "\t";
  os << " page faults= " << page_faults << "\t";
  os << " minor faults= " << minor_page_faults << "\t";
  os << " major faults= " << major_page_faults << "\t";
  os << " blocked input= " << blocked_input_ops << "\t";
  os << " blocked output= " << blocked_output_ops << "\t";
*/
}
