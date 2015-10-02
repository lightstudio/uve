using System;
using System.Diagnostics;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Controls;
using UVEngineNative;

namespace UVEngine2_1.Classes
{
    public static class Benchmark
    {
        public static async Task<double> CPUSingle()
        {
            var cpuSingle = BenchCPU.CalcPiSingle(100);
            cpuSingle.Progress =
                (asyncInfo, progressInfo) =>
                {
                    BenchmarkRuntime.CurrentBenchmark.Progress = progressInfo;
                };
            var time = await CPUSingle();
            return time;
        }
        public static async Task<double> CPUMulti()
        {
            var cpuMulti = BenchCPU.CalcPiSingle(300);
            cpuMulti.Progress =
                (asyncInfo, progressInfo) =>
                {
                    BenchmarkRuntime.CurrentBenchmark.Progress = progressInfo;
                };
            var time =  await cpuMulti;
            return time;
        }

        public static async Task<double> Memory()
        {
            var memory = BenchMem.Bench(128);
            memory.Progress =
                (asyncInfo, progressInfo) =>
                {
                    BenchmarkRuntime.CurrentBenchmark.Progress = progressInfo;
                };
            var time = await memory;
            return time;
        }
    }
}