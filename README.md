# EightEngine
Custom built engine on top of DirectX 11 and Nvidia PhysX

Resource Cache

Resource cache works on premise of handlers and loaders. New resources are automatically cooked into engine specific format. When cooked resource is loaded, it will be pushed through specific loader. Loader will change this resource into real representation(e.g. from .dds file will engine create texture on D3D11 side). If we are loading new resource which engine doesn't know, binary data will be saved and warning message will be shown. Loader will create on top of loaded resource handler. Handler is an unique instance of specific resource saved in resource cache. Therefore there can not be multiple instance of same resource. Handlers are working similarly as shared smart pointers. I didn't use std::shared_ptr<> because I've wanted to have more control about allocations and especially merged allocation for resource data and handler data. Memory for resource cache works on blocks which are automatically prealocated. You can completely configure memory system. Whether it can allocate more memory or how mach memory can be used etc. Resource cache in cooked game loads many resources at once. This resources are saved in custom file known as .PAK file. I choose this design because it is faster to load one big file as many small ones. If somebody asks for resource from resource cache, resource handle will be returned. If such resource doesn't exist, default resource of specified type will be returned(e.g. texture will be rendered as black square).

Memory

Eight engine has a custom memory manager, which overrides default allocator. Memory manager keeps track about memory allocations(how much memory was allocated etc.). Memory pools, for faster allocations, are currently used for events and resources.

Actors

All objects which can be spawned into the game world must be instances of actor(or any child). Actor is used as a container, which holds specific number of components which define actor behavior. Every actor is created from configuration file(.xml).

Audio

Eight engine uses XAudio for playing sounds. There is an independent layer between XAudio and engine, therefore audio framework can be changed without modification of other subsystems. Currently are supported only 2D sounds.

Logging

Eight engine uses four levels of logging. First level is log. It's just informative message(e.g. Actor was spawned). Second level is warning. It's telling you that something is wrong(e.g. requested loader doesn't exist). Third level is error. It's telling you, that something is wrong and needs to be immediately fixed, but doesn't cause crash(e.g. resource which you want to load is corrupted). The last level is assert, which will cash game.

Events

Events create communication channel between different subsystems. They are simple messages which are used to increase decoupling. Every object which wants to listen to specific events, needs to register yourself as event listener for all types, he wants to listen to. Events are allocated inside memory pools, therefore their creation is almost free.

Graphics

Eight engine uses DirectX 11 for rendering actors, but there is an independent layer between DirectX 11 and engine, therefore it can be changed without modification of other subsystems. Eight engine can render in 2D and 3D mode. It currently supports rendering of Sky, Terrain, Models, which consists from meshes and materials, and User Interface. Rendering systems uses rendering scene for sorting and culling actors. Rendering scene consists from rendering passes which renders all visible actors of specific type(e.g. Models). It uses brute force frustum culling to cull out actors which are not visible. Batched rendering is supported.

Physics

Eight engine uses PhysX for computing physics. There are supported 3 types of actors. Static actors, dynamic actors which are moving in physically correct way by forces and impulses, and kinematic actors. Eight engine support two types of collision. Hitting blocking volume or trigger. Triggers are invisible actors which activates specific logic on contact.

Processes

Eight engine supports dividing tasks into small blocks. Currently it supports only preemptive multitasking, but architecture is ready for running in parallel.
