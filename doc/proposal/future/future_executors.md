<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXX</span>=yy-nnnn</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2015-11-22</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Project:</td>
        <td width="435">ISO/IEC JTC1 SC22 WG21 Programming Language C++</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Audience:</td>
        <td width="435">Library Evolution Working Group</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escriba &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@wanadoo.fr</a>&gt;</td>
    </tr>
</table>

Adding Executor Aware Constructors for `promise<T>`
===================================================

# Introduction

[] proposes a `spawn` function that constructs futures associated to an `Executor`. 
The `then()` member function uses this executor to launch the continuation.

But futures can also have other sources, as e.g. a `promise` or a `packaged_task`. 
This paper proposes the addition of `Executor` aware constructors for `promise<T>` and `packaged_task<R(Args)>`
so that the continuation on the associated future make use of this executor.

# Motivation and Scope




# Proposal

We propose to: 
* Add `promise<T>::promise<T>(Executor&)` Executor aware constructor.

* Add `packaged_task<R(Args...)>::packaged_task(Executor&)` Executor aware constructor.

# Design rationale

## Associating the `Executor` at construction time

This paper proposes to associate the Executor at construction time so that the implementation can construct a specific shared state that depends on Executor.

## `set_executor(Excecutor&)` versus constructors

The shared state associated to a `promise<T>` can be created lazily, when we set a value of type T or get the associated `future<T>`.
This associated state only depends on type `T`.

We can have a `set_executor(Executor&)` that would be able to create the shared state if not already created. 
This shared state would depends on type `T` and type `Executor`.
That means that once we have an associated state that depends on T we can not set an executor. 
This proposal doesn't propose this `set_executor`function.

An alternative is to type-erase the `Executor` type on the shared state, so that we can set an executor at any time.

# Open questions

## Do we want to `set_executor` function?  with his limitation? or

## Do we want to type-erase the `Executor` stored in the shared state?

## If `set_executor` is preferred, wouldn't be better to associated this function to `future<T>`? 

## Do we need to add an `Executor` parameter to `make_ready_future` and  `make_exceptional_future`

# Proposed Wording

The wording is relative to [N4538] 
 
Update **X.Y Header <experimental/future> synopsis adding**

# Implementability

This proposal can be implemented as pure library extension, without any compiler magic support, in C++14.
An almost full reference implementation of this proposal can be found at [Boost.Thread].

# Future work
   
# Acknowledgements 

Thanks to Vladimir Prus who presented the use case on the Boost ML.

# References


[N4538]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4538.pdf "Working Draft,  Technical Specification for C++ Extensions for Concurrency, Version 1"  


* [N4538] - Working Draft,  Technical Specification for C++ Extensions for Concurrency, Version 1  
 
	http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4538.pdf

# use_case 


