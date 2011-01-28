/* Job.hpp

Copyright (c) 2010 - 2011 by Felix Lauer and Simon Schneegans

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>. */

# ifndef JOB_HPP_INCLUDED
# define JOB_HPP_INCLUDED

struct Job {
    enum JobType{jLand, jCharge, jHeal, jAssist, jUnfreeze, jAttackAny,
                 jAttackTarget, jGetPUFuel, jGetPUHealth, jGetPUReverse,
                 jGetPUShield, jGetPUSleep, jKickOutHome, jKickToEnemy,
                 jWaitForBall, jProtectZone};

    Job(JobType type, short priority, void* object = NULL):
         type_(type),
         priority_(priority),
         object_(object) {}

    JobType type_;
    short priority_;
    void* object_;


    bool operator< (Job const& rhs) const {
        return priority_ < rhs.priority_;
    }

    bool operator> (Job const& rhs) const {
        return priority_ > rhs.priority_;
    }
};



# endif // JOB_HPP_INCLUDED





