<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXX</span>=yy-nnnn</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2015-01-11</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Project:</td>
        <td width="435">Programming Language C++, Library Evolution Working Group</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escriba &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@wanadoo.fr</a>&gt;</td>
    </tr>
</table>

Adding Emplace Factories for `promise<T>/future<T>/exception_ptr`
===================================================

# Introduction

This paper proposes the addition of emplace factories for `future<T>` as we have proposed for of `any` and `optional` in [P0032R0], 

# Motivation and Scope

While we have added the `future<T>` factories `make_ready_future` and `make_exceptional_future` into [N4256], 
we don't have emplace factories as we have for `shared_ptr`and `unique_ptr` and we could have for `any` and `optional` if [P0032R0] is accepted.  

The C++ standard should be coherent for features that behave the same way on different types and complete, that is, 
don't miss features that could make the user code more efficient. 

# Proposal

We propose to: 
* Add `promise<T>::set_exception(E)` member function that sets a promise `exception_ptr` from an exception.

* Add `promise<T>::emplace(Args...)` member function that emplaces the value instead of setting it.

* Add `promise<T>::emplace_exception<E>(Args...)`  member function that emplaces the exception `E` instead of setting it.

* Add `exception_ptr` emplace factory `emplace_exception_ptr<E>(Args...)` that emplaces any 
exception on an `exception_ptr` instead of moving it.

* Add `future<T>` emplace factory `emplace_ready_future<T>(Args...)`/`make_ready_future<T>(Args...)`.

* Add `future<T>` emplace factory `emplace_exceptional_future<T,E>(Args...)/make_exceptional_future<T,E>(Args...)`.

# Design rationale

## emplace_ versus make_ factories

# Technical Specification

The wording is relative to [nXXXX] 

Update X.Y Header <experimental/exception_ptr> synopsis adding

Update X.Y Header <experimental/future> synopsis adding

   
# Acknowledgements 

Thanks to 

# References

## nXXXX - Working Draft, C++ Extensions for Library Fundamentals 


## P0032R0 -  Homogeneous interface for variant, any and optional 
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0032r0.pdf

## DXXXX - C++ generic factory 
https://github.com/viboes/std-make/blob/master/doc/proposal/factories/DXXXX_factories.md

## C++ generic factory - Implementation 
https://github.com/viboes/std-make/blob/master/include/experimental/std_make_v1/make.hpp


