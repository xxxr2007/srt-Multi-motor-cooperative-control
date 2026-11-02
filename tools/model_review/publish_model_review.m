function publish_model_review(manifestFile)
% Export each side versus the common baseline. Never simulate or save models.
manifest = jsondecode(fileread(manifestFile));
outputFolder = fileparts(manifestFile);
s = settings().comparisons.slx.DisplayReportScreenshots;
s.TemporaryValue = false;
results = struct('name', {}, 'success', {}, 'report', {}, 'error', {}, 'warning', {});
failed = false;
for k = 1:numel(manifest.changes)
    entry = manifest.changes(k);
    [~,~,extension] = fileparts(entry.file);
    if isempty(entry.old) || isempty(entry.new) || ~ismember(lower(extension), {'.slx','.mdl'})
        continue;
    end
    result = struct('name', entry.native_name, 'success', false, 'report', '', 'error', '', 'warning', '');
    lastwarn('');
    try
        comparison = visdiff(entry.old, entry.new);
        result.report = publish(comparison, 'Format', 'HTML', 'Name', entry.native_name, 'OutputFolder', outputFolder);
        result.success = true;
        [result.warning,~] = lastwarn;
    catch exception
        result.error = getReport(exception, 'extended', 'hyperlinks', 'off');
        failed = true;
    end
    results(end+1) = result; %#ok<AGROW>
end
fid = fopen(fullfile(outputFolder,'native_results.json'),'w','n','UTF-8');
assert(fid ~= -1, 'Cannot write native_results.json');
cleanup = onCleanup(@() fclose(fid));
fwrite(fid, jsonencode(results, 'PrettyPrint', true), 'char');
clear cleanup;
if failed
    error('ModelReview:ExportFailed', 'One or more native exports failed. Read native_results.json.');
end
end
