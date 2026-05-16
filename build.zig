const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const testHeader = b.addTranslateC(.{
        .target = target,
        .optimize = optimize,
        .root_source_file = b.path("test/unitsTests.h"),
        .link_libc = true,
    });

    const testModule = b.addModule("tests", .{
        .link_libcpp = true,
        .target = target,
        .optimize = optimize,
        .root_source_file = b.path("./test/root.zig"),
    });
    testModule.addIncludePath(b.path("src"));
    testModule.addCSourceFile(.{
        .file = b.path("./test/unitsTests.cpp"),
        .language = .cpp,
        .flags = &.{},
    });

    testModule.addImport("header", testHeader.addModule("header"));
    const tests = b.addTest(.{
        .root_module = testModule,
        .name = "Units Tests",
    });

    const testStep = b.step("test", "Run tests");
    const runTests = b.addRunArtifact(tests);
    testStep.dependOn(&runTests.step);

    const writeStep = b.addWriteFiles();
    const emptyFile = writeStep.add(
        "headerroot.zig",
        "pub fn donotusethisfunction() void {}",
    );

    const headerMod = b.addModule("headermod", .{
        .target = target,
        .optimize = optimize,
        .root_source_file = emptyFile,
    });
    headerMod.addIncludePath(b.path("src"));

    const mainMod = b.addModule("main", .{
        .optimize = optimize,
        .target = target,
        .root_source_file = null,
        .link_libcpp = true,
    });
    mainMod.addCSourceFile(.{
        .file = b.path("demo/main.cpp"),
    });
    mainMod.addIncludePath(b.path("src"));

    const exe = b.addExecutable(.{
        .name = "units",
        .root_module = mainMod,
    });

    const run = b.addRunArtifact(exe);
    const runStep = b.step("run", "Run");
    runStep.dependOn(&run.step);
    if (b.args) |arg| {
        run.addArgs(arg);
    }

    b.installArtifact(exe);
}
