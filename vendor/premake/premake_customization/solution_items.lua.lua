-- Implement the solution_items command for solution-scope files
require('vstudio')

local vs = premake.vstudio.vc2010

local function premakeVersionComment(prj)
    premake.w('<!-- Generated by Premake ' .. _PREMAKE_VERSION .. ' -->')
end

local function vcpkg(prj)
    premake.w('<VcpkgTriplet>x64-windows-static</VcpkgTriplet>')
    premake.w('<VcpkgEnabled>false</VcpkgEnabled>')
    premake.w('<VcpkgManifestInstall>false</VcpkgManifestInstall>')
    premake.w('<VcpkgAutoLink>false</VcpkgAutoLink>')  
end

premake.override(premake.vstudio.vc2010.elements, "project", function(base, prj)
    local calls = base(prj)
    table.insertafter(calls, vs.xmlDeclaration, premakeVersionComment)
    return calls
end)

premake.override(premake.vstudio.vc2010.elements, "globals", function(base, prj)
    local calls = base(prj)
    table.insertafter(calls, vs.globals, vcpkg)
    return calls
end)

premake.api.register {
	name = "solution_items",
	scope = "workspace",
	kind = "list:string",
}

premake.override(premake.vstudio.sln2005, "projects", function(base, wks)
	if wks.solution_items and #wks.solution_items > 0 then
		local solution_folder_GUID = "{2150E333-8FDC-42A3-9474-1A3956D46DE8}" -- See https://www.codeproject.com/Reference/720512/List-of-Visual-Studio-Project-Type-GUIDs
		premake.push("Project(\"" .. solution_folder_GUID .. "\") = \"Solution Items\", \"Solution Items\", \"{" .. os.uuid("Solution Items:" .. wks.name) .. "}\"")
		premake.push("ProjectSection(SolutionItems) = preProject")

		for _, path in ipairs(wks.solution_items) do
			premake.w(path .. " = " .. path)
		end

		premake.pop("EndProjectSection")
		premake.pop("EndProject")
	end
	base(wks)
end)