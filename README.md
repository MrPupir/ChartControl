# ChartControl

An ActiveX control written in C++ with MFC. It draws a column chart from rows passed in by the host application and exports the same data to Microsoft Excel or Word through COM automation. It is the chart component of the admin statistics panel in [MagicCupGame](https://github.com/MrPupir/MagicCupGame), my bachelor's diploma project (State University of Economics and Technology, Kryvyi Rih, 2026).

## Features

- Draws a 2D or 3D column chart with GDI into an off-screen bitmap. In 3D mode each bar gets a lighter top face and a darker side face. The chart has a title, a Y axis from 0 to the largest value in five equal steps, an optional dotted grid, and configurable axis, grid and background colours.
- `LoadData` takes an array of rows `[label, value1, value2]`: a BSTR label followed by `VT_R8` or `VT_I4` numbers. A missing or non-numeric value counts as 0. Each bar is `CoefA * value1 + CoefB * value2` (defaults 0.6 and 0.4) and is recalculated when a coefficient changes. `Clear` removes the data.
- Bar colours come from the `Colors` property, an array of COLORREF values used in a cycle. Each colour stays with its row when the chart is sorted. Without the property, three default colours are used.
- Each value label goes inside its bar, rotated inside the bar, or above it, whichever fits. Category labels turn 90 degrees when they do not fit under the bars, and `ShowLabels` hides them.
- `Sort(TRUE)` toggles ascending order on and off, and `Sort(FALSE)` does the same for descending order. Only an index array is sorted. Loading data or changing a coefficient returns the chart to the original order.
- `ExportToExcel` adds a "Report" sheet to a new workbook. The sheet holds a formatted Category/Value table, converted to an Excel table, and a column chart with the control's title, bar colours and data labels.
- `ExportToWord` creates a document with the title, a two-column table and an embedded column chart built from the same rows. The Word chart uses Word's default colours and has no data labels. Both exports follow the current sort order and the 3D setting.
- Raises `OnChartClick` on any left click and `OnExportFinished` after an export. `ChartTitle`, `ShowLabels`, `CoefA`, `CoefB`, `Enable3D`, `ShowGrid`, `GridColor`, `AxisColor` and `BackColor` are saved with the control.

## Tech stack

- C++ and MFC (`COleControl`), built as an in-process ActiveX control (`ChartControl.ocx`)
- COM / OLE Automation: a MIDL type library with a dispatch interface and an event interface
- GDI: memory DC, world transforms for rotated text
- Excel and Word object models through `#import`-generated wrappers (`comdef.h` smart pointers)
- Visual Studio 2022, platform toolset v143, Win32 and x64 configurations

## Getting started

Prerequisites:

- Windows
- Visual Studio 2022 with the Desktop development with C++ workload and the MFC component for the v143 build tools
- Microsoft Office with Excel and Word. The exports drive them at runtime, and the build needs their type libraries.

1. **Generate the Office wrapper headers.** `pch.h` includes `MSO.tlh`, `VBE6EXT.tlh`, `EXCEL.tlh` and `MSWORD.tlh` from the project folder. `.gitignore` excludes `*.tlh` and `*.tli`, so these files are not in the repository. In the project folder, create a scratch file `office_import.cpp` with the lines below. The paths are the Office 16 paths the original headers were generated from, so change them to match your Office installation. The renames avoid clashes with Windows macros.

   ```cpp
   #import "C:\Program Files (x86)\Microsoft Office\root\vfs\ProgramFilesCommonX86\Microsoft Shared\OFFICE16\MSO.DLL" rename("RGB", "RGBXL")
   #import "C:\Program Files (x86)\Microsoft Office\root\vfs\ProgramFilesCommonX86\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"
   #import "C:\Program Files (x86)\Microsoft Office\root\Office16\EXCEL.EXE" rename("DialogBox", "DialogBoxXL") rename("RGB", "RGBXL")
   #import "C:\Program Files (x86)\Microsoft Office\root\Office16\MSWORD.OLB" rename("ExitWindows", "WordExitWindows") rename("FindText", "WordFindText") rename("RGB", "RGBWORD")
   ```

   From a Developer Command Prompt for VS 2022 in the same folder, run `cl /c /EHsc office_import.cpp`. The compiler writes the four `.tlh` and four `.tli` files next to `pch.h`. Then delete `office_import.cpp` and `office_import.obj`.
2. **Build.** Open `ChartControl.sln` and build Debug or Release for x86 or x64. Pick the platform that matches the host application, because an in-process control loads only into a process of the same bitness. The output is `Release\ChartControl.ocx` for x86 and `x64\Release\ChartControl.ocx` for x64 (`Debug\` and `x64\Debug\` for Debug builds).
3. **Register.** The project's Register Output linker option registers the control after the build. This needs Visual Studio running as administrator. If it is not, run `regsvr32 x64\Release\ChartControl.ocx` (or the x86 path) from an elevated prompt.
4. **Use it.** In an MFC dialog, use Insert ActiveX Control and pick "ChartControl Control" (ProgID `MFCACTIVEXCONTRO.ChartControlCtrl.1`). Generate a wrapper class (MagicCupGame's is [`ChartControl.h`](https://github.com/MrPupir/MagicCupGame/blob/master/ChartControl.h)), set the properties, then call `LoadData`. `CAdminDlg::RefreshChart` in MagicCupGame's [`AdminDlg.cpp`](https://github.com/MrPupir/MagicCupGame/blob/master/AdminDlg.cpp) builds the row and colour arrays this way.

## Project structure

- `ChartControlCtrl.cpp`, `ChartControlCtrl.h`: the control (properties and methods, drawing, sorting, Excel and Word export)
- `ChartControl.idl`: type library with `_DChartControl`, `_DChartControlEvents` and the `ChartControl` coclass
- `ChartControl.cpp`, `ChartControl.def`: DLL module with `DllRegisterServer` and `DllUnregisterServer`
- `pch.h`: precompiled header that pulls in the Office wrappers
- The remaining files are MFC ActiveX wizard output: resources, an empty property page, and `ChartControlidl.h` generated by MIDL.

## Author

Kyrylo Radchenko ([MrPupir](https://github.com/MrPupir))

Bachelor's diploma project (chart component of MagicCupGame), State University of Economics and Technology, Kryvyi Rih, 2026.
