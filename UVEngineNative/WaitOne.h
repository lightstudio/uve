
template <typename TResult>
inline TResult AWait(Windows::Foundation::IAsyncOperation<TResult>^ asyncOp)
{
	HANDLE handle = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
	TResult result;
	asyncOp->Completed = ref new Windows::Foundation::AsyncOperationCompletedHandler<TResult>([&]
		(Windows::Foundation::IAsyncOperation<TResult>^ asyncInfo, Windows::Foundation::AsyncStatus asyncStatus)
	{
		result = asyncInfo->GetResults();
		SetEvent(handle);
	});
	WaitForSingleObjectEx(handle, INFINITE, FALSE);
	return result;
}