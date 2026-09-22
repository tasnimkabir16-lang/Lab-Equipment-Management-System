# Lab Equipment Issue & Return Management System

A simple lab equipment management system designed to make issuing, returning, and tracking shared laboratory equipment easier.

The system supports three different user roles: **Student, Lab Officer, and Department Admin**. Each role has its own set of features and permissions. The project includes a C++ console-based backend implementation and a browser-based frontend interface.

## Features

### Student

Students can:

* Log in to their account
* View available lab equipment
* Request equipment
* Return previously issued equipment
* View their borrowing history
* Update their profile information

The system keeps track of equipment requests, issue dates, due dates, quantities, and request status.

### Lab Officer

Lab Officers can:

* View pending equipment requests
* Approve equipment requests
* Reject requests
* Verify returned equipment
* Record damaged or lost equipment
* Update their profile

When a request is approved, the available equipment quantity is automatically updated. Returned equipment can also be checked before being added back into inventory.

### Department Admin

Department Admins can:

* Add new equipment
* Update existing equipment
* Delete equipment
* Check equipment availability
* View the complete inventory
* Manage registered users
* View issue and return reports
* View overdue reports
* View damaged equipment reports

## Equipment Tracking

Each piece of equipment contains information such as:

* Equipment ID
* Equipment name
* Category
* Status
* Available quantity

Equipment can have statuses such
