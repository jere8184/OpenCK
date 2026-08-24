very early days, this will never be finished, working on parser, scripting and traits, feel free to join and contribute.

Currently has a parser that can parse pdx-script into AST of nodes. These nodes can used to create the various game objects, see trait.cpp .
I'm currently working on a byte code compiler which compiles AST of script into bytecode to allow for execution of script during run time. 

Below is some of the example byte code produced when compiling Trait 'potential' blocks.

excommunicated: NOT || controls_religion || LOAD_TRUE || RETURN || RETURN ||

kinslayer: NOT || religion_group || muslim || RETURN || is_tribal || LOAD_FALSE || RETURN ||

scholar: NOT || religion_group || muslim || RETURN || RETURN ||

impaler: NOT || religion || jain || RETURN || RETURN ||

hunter: NOT || religion || jain || RETURN || RETURN ||

falconer: NOT || religion || jain || RETURN || RETURN ||

Feel free to contribute.
