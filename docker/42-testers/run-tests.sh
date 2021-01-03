valgrind --track-fds=yes ./webserv -f conf2.json &> valgrind_out.log &
jobpid=$!

while ! grep 'Command:' valgrind_out.log
do
	echo 'waiting for server to start...'
	sleep 1;
done

./ubuntu_tester http://localhost:80
out=$?

printf "\n\n\n-- sending an interrupt --\n"
kill -s INT $jobpid

echo "waiting for program exit"
wait $jobpid

printf "\n\n\n-- valgrind logs --\n"
cat valgrind_out.log

exit $out