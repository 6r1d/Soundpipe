sptbl["delay"] = {

    files = {
        module = "delay.c",
        header = "delay.h",
        example = "ex_delay.c",
    },

    func = {
        create = "sp_delay_create",
        destroy = "sp_delay_destroy",
        init = "sp_delay_init",
        compute = "sp_delay_compute",
    },

    params = {
        mandatory = {
            {
                name = "time",
                type = "SPFLOAT",
                description = "Delay time, in seconds.",
                default = 1.0
            }
        },
    },

    modtype = "module",

    description = [[Add a delay to an incoming signal.]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "input",
            description = "Signal input."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}
